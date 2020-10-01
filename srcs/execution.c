/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/01 22:13:07 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <minishell.h> // rm this when i could include without error builtins.h
//#include <builtins.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Have to add the libft for have the libc fcts */

size_t		token_count(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens)
	{
		//ft_printf("token count [%p]\n", tokens);
		count++;
		tokens = tokens->next;
	}
	return (count);
}

static bool			is_builting(const int ac, const char **argv, t_data *data)
{
	const char		*name = argv[0];

	if (!ft_strncmp(name, "echo", 5))
		data->return_status = ft_echo(ac, argv);
	else if (!ft_strncmp(name, "cd", 3))
		data->return_status = ft_cd(ac, argv, data);
	else if (!ft_strncmp(name, "pwd", 4))
		data->return_status = ft_pwd();
	else if (!ft_strncmp(name, "export", 7))
		data->return_status = ft_export(ac, argv, data);
	else if (!ft_strncmp(name, "unset", 6))
		data->return_status = ft_unset(ac, argv, data);
	else if (!ft_strncmp(name, "env", 4))
		data->return_status = ft_env(data->env); // should ve a t_map
	else if (!ft_strncmp(name, "exit", 5))
		data->return_status = ft_exit(data);
	else
		return (false);
	return (true);
}

/* I'm modifying data->envp, probally a bad idea TO SEE LATER*/

/* Normally witouth redirecting the fd, dont need to fork */
bool				execute_simple_cmd(t_bst *curr, t_data *data) // tokenize -> token -> 
{
	char			*execution_path;
	char			**envp;

	if (!is_builting(curr->ac[0], argv, data))
	{
		if (!(execution_path = path_get(*curr->av[0], \
				map_get(data->env, "PATH")->value)))
			return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", *curr->av[0]) & \
					free_four_ptrs_and_bst(NULL, NULL, NULL, NULL, &curr));
		if (!(envp = map_export(data->env)))
			return (ft_dprintf(2, "could not export environment!") & \
					free_four_ptrs_and_bst(NULL, &execution_path, NULL, NULL, &curr));
		data->return_status = execve(execution_path, curr->av[0], envp);
		ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
		return (false);
	}
	free_four_ptrs_and_bst(NULL, &execution_path, &envp, NULL, &curr);
	return (true);
}

static bool			open_and_dup_stdio(t_bst *head)
{
	int				fd;

	if (head->operator & REDIRECTION_GREATHER)
	{
		if ((fd = open(head->cmd[1], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	else if (head->operator & REDIRECTION_LESSER)
	{
		if ((fd = open(head->cmd[1], O_RDONLY)) < 0 \
				|| dup2(fd, STDIN_FILENO) < 0)
			return (false);
	}
	else if (head->operator & REDIRECTION_DGREATHER)
	{
		if ((fd = open(head->cmd[1],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	return (!(close(fd) < 0));
}

bool				execute_redirections_cmd(t_bst *curr, t_data *data)
{
	char			*execution_path;
	char			**envp;

	if (!(data->pid = fork()))
	{
		if (!open_and_dup_stdio(curr))
			return (false);
		if (!is_builting(curr->ac[0], argv, data))
		{
			if (!(execution_path = path_get(*curr->av[0], \
				map_get(data->env, "PATH")->value)))
				return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", curr->av[0]) & \
					free_four_ptrs_and_bst(NULL, NULL, NULL, NULL, &curr));
			if (!(envp = map_export(data->env)))
				return (ft_dprintf(2, "could not export environment!") & \
				free_four_ptrs_and_bst(NULL, &execution_path, NULL, NULL, &curr));
			data->return_status = execve(execution_path, curr->av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
		else if (data->pid < 0)
			return (!(data->return_status = 127));
		while (waitpid(data->pid, NULL, 0) <= 0)
			;
		data->pid = 0;
		free_four_ptrs_and_bst(NULL, &execution_path, &envp, NULL, &curr);
		return (true);
	}
}

static bool			execute_child_process(t_pipe2 *p, char **av, uint32_t ac, t_data *data)
{
	char			execution_path;
	char			**envp;

	if (!(data->pid = fork()))
	{
		if (p->in[0])
		{
			dup2(p->in[0], STDIN_FILENO);
			close(p->in[0]);
		}
		if (p->fd[1] != STDOUT_FILENO)
		{
			dup2(p->fd[1], STDOUT_FILENO);
			close(p->fd[1]);
		}
		if (!is_builting(ac, argv, data))
		{
			if (!(execution_path = path_get(av[0], \
				map_get(data->env, "PATH")->value)))
				return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", av[0]) & \
					free_four_ptrs_and_bst(NULL, NULL, NULL, NULL, NULL));
			if (!(envp = map_export(data->env)))
				return (ft_dprintf(2, "could not export environment!") & \
				free_four_ptrs_and_bst(NULL, &execution_path, NULL, NULL, NULL));
			data->return_status = execve(execution_path, av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			exit(0); // to test
			return (false);
		}
	}
	else if (data->pid < 0)
		return (!(data->return_status = 127));
	while (waitpid(data->pid, NULL, 0) < 0)
		;
	data->pid = 0;
	free_five_ptrs(&execution_path, NULL, &envp, NULL, NULL);
	return (true);
}

bool				execute_pipe_cmd(t_bst *curr, t_data *data)
{
	t_pipe2			p;
	bool			once;

	p.in[0] = 0;
	once = true;

	while (curr)
	{
		pipe(p.fd);
		if (!execute_child_process(&p, curr->av[once ? 0 : 1], curr->ac[once ? 0 : 1], data) \
				|| close (p.fd[1]) < 0)
			return (false);
		p.in[0] = p.fd[0];
		if (!once)
			curr = curr->next;
		if (!curr->next || (curr->next && !(curr->next->operator & PIPE)))
			break ;
		once = false;
	}
	if (p.in[0] != 0)
		dup2(p.in[0], STDIN_FILENO);
	/* Seems work for:
			cmd | cmd,
			cmd | cmd | cmd,
			cmd | cmd | cmd | cmd,
			cmd | cmd > file and
			cmd | cmd | cmd > file

		The previous while will loop until the last cmd, this last cmd will
				be executed or redirected bellow
	*/	
	if (curr->next && (curr->next->operator & REDIRECTION_GREATHER \
			|| curr->next->operator & REDIRECTION_DGREATHER \
			|| curr->next->operator & REDIRECTION_LESSER))
	{
		curr->av[0] = curr->av[1];
		curr->av[1] = curr->next->av[1];
		execute_redirections_cmd(curr, data);
	}
	else
	{
		curr->av[0] = curr->av[1];
		execute_simple_cmd(curr, data);
	}
	return (true);
}
