/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 20:21:37 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/04 20:15:38 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <minishell.h> // rm this when i could include without error builtins.h
//#include <builtins.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

static bool			open_and_dup_fd(t_bst *head, int src_fd)
{
	int				fd;

	if (head->operator & REDIR_GR)
	{
		if ((fd = open(head->av[1], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, src_fd) < 0)
			return (false);
	}
	else if (head->operator & REDIR_LE)
	{
		if ((fd = open(head->av[1], O_RDONLY)) < 0 \
				|| dup2(fd, src_fd) < 0)
			return (false);
	}
	else if (head->operator & REDIR_DG)
	{
		if ((fd = open(head->av[1],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, src_fd) < 0)
			return (false);
	}
	return (!(close(fd) < 0));
}

static bool			OLD______pipe_execution(t_pipe *p, t_bst *curr, t_term *term, int cmd)
{
	char			execution_path;
	int				ac;
	char			**argv;
	char			**envp;

	if (!(argv = token_tab(curr->cmd[cmd] , &ac)))
		return (false);
	if (!(data->pid = fork()))
	{
		dup(STDOUT_FILENO);
		if (p->in && dup2(p->fd_read, STDIN_FILENO) < 0)
			return (false);
		if (p->out && dup2(p->fd_write, STDOUT_FILENO) < 0)
			return (false);
		if (!is_builtin(ac, argv, data))
		{
			if (!(execution_path = path_get(argv[0], \
				map_get(data->env, "PATH")->value)))
				return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", data->argv[0]) & \
					free_four_ptrs_and_bst(&argv, NULL, NULL, NULL, &curr));
			if (!(envp = map_export(data->env)))
				return (ft_dprintf(2, "could not export environment!") & \
				free_four_ptrs_and_bst(&argv, &execution_path, NULL, NULL, &curr));
			data->return_status = execve(execution_path, argv + 1, envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
	}
	else if (data->pid < 0)
		return (!(data->return_status = 127));
	while (waitpid(data->pid, NULL, 0) < 0)
		;
	data->pid = 0;
	ft_bzero(p, 64); // wanna erase to '/0' the 2 first integers of p
	free_five_ptrs(&execution_path, &argv, &envp, NULL, NULL);
	return (true);
}

bool				OLD________execute_pipe_cmd(t_bst *curr, t_term *term)
{
		/* TO DO, 
		ONLY (a little) DIFFICULT STUFF IN THIS PROJECT:
		cmd | cmd | cmd or
		cmd | cmd | cmd > file
		*/
		/* Architecture:
			1) pipe take stdout
			2) redirect stdout to stdin
			3) if redirection redirect stdout to file
			Repeat for each pipe
		*/

	/* THERE IS AN EXAMPLE OF PIPES USE FOR COMUNICATION BETWEEN CHILD AND PARENT IN THE MAN */

	/* https://aljensencprogramming.wordpress.com/2014/05/16/implementing-pipes-in-c/ */
	t_pipe			p;

	pipe(p.fd);
	p.fd_write = p.fd[1];
	p.fd_read = p.fd[0];
	p.in = 0;
	p.out = 1;
	if (!(pipe_execution(&p, curr, data, 0)) || close(p.fd_write) < 0)
		return (false);
	p.in = 1;
	p.out = 0;
	p.fd_read = p.fd[0];
	// probally have to loop here if the next is a pipe and increment curr
	if (!(pipe_execution(&p, curr, data, 1)) || close(p.fd_write) < 0 \
		|| close(p.fd_read) < 0)
		return (false);
	if (curr->next && curr->next->operator & PIPE)
	{
		/* send fd to next pipe if i don t loop */
	}
	else if (curr->next && (curr->next->operator & REDIR_GR \
			||curr->next->operator & REDIR_DG \
			|| curr->next->operator & REDIR_LE) \
			&& !open_and_dup_fd(curr->next, p.fd_write))
		return (false);
	/* If it doesn't work i can put open_and_dup_fd before the last pipe execution */
	return (true);
}
