/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/18 18:04:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static int	handle_status(int status)
{
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		dprintf(2, "[exec][status] child exited with status '%d'!\n", status);
		return (status);
	}
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		dprintf(2, "[exec][status] %s\n", strsignal(status));
		return (status);
	}
	dprintf(2, "[exec][status] cannot retrieve child status!\n");
	return (status);
}

static int			exec_and_join(t_args *args, t_term* t)
{
	int	status;

	if (!(t->pid = fork()))
	{
		dprintf(2, "[exec][child] executing '%s'...\n", (const char *)args->exec); // DEBUG
		if (dup_stdio(args->fds))
		{
			status = execve((const char *)args->exec, args->av, args->ep);
			ft_dprintf(2, "%s: %s: execve returned '%d'!\n", t->name, args->av[0], status);
			ft_dprintf(2, "[errno] %d: %s\n", errno, strerror(errno)); // DEBUG
		}
		else
			ft_dprintf(2, "[exec][child][dup] failed!\n");
		exit(EXIT_FAILURE);
	}
	else if (t->pid < 0)
	{
		ft_dprintf(2, "[exec][parent] error: pid is '%d'!\n", t->pid); // DEBUG
		ft_dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
		return (errno);
	}
	ft_dprintf(2, "[exec][parent] joining child with pid '%d'!\n", t->pid); // DEBUG
	while (waitpid(t->pid, &status, 0) <= 0)
		;
	ft_dprintf(2, "[exec][parent] joined child!\n");
	return (handle_status(status));
}

static t_executable	exec_get(t_args *args, t_term *term)
{
	const char			*names[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	const int			lengths[] = {4, 3, 4, 7, 5, 4, 5};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, &ft_unset, &ft_env, &ft_exit};
	int					i;

	i = 0;
	while (i < 7 && ft_strncmp(args->av[0], names[i], lengths[i]))
		i++;
	if (i < 7)
		return (builtins[i]);
	if (get_path_and_envp((char **)&args->exec, (char ***)&args->ep, args->av[0], term))
		return (&exec_and_join);
	return (NULL);
}

bool				exec_cmd(t_args *args, t_term *term)
{
	const t_executable	exec = exec_get(args, term);

	if (exec)
		term->st = exec(args, term);
	else
	{
		ft_dprintf(2, "[exec][cmd] '%s': File not found!\n", args->av[0]); // DEBUG
		return (false);
	}
	close_fds(args->fds);
	return (true);
}

bool				exec_pipe_cmd(t_bst *curr, t_term *term, int in_fd, int index)
{
	t_args	args;
	int		pipe_fds[2]; // pipes

	if (curr)
	{
		if (!curr->next || (curr->next && curr->next->operator & PIPE))
		{
			args.fds[0] = in_fd;
			args.fds[1] = STDOUT_FILENO;
			args.fds[2] = STDERR_FILENO;
		}
		else if (curr->next && !redir_fds(args.fds, curr->next->av[1] ? curr->next->av[1][0] : NULL, curr->next->operator, in_fd))
			return (false);
		args.ac = curr->ac[index];
		args.av = curr->av[index];
		if (index == 0 || (curr->next && (curr->next->operator & PIPE || (curr->next->next \
				&& (curr->next->operator & REDIR_GR || curr->next->operator & REDIR_DG || curr->next->operator & REDIR_LE)))))
		{
			if (pipe(pipe_fds) < 0)
			{
				ft_dprintf(2, "[pipe][error] pipe returned '-1'!\n"); // DEBUG
				ft_dprintf(2, "[errno] %d: %s\n", errno, strerror(errno)); // DEBUG
				return (false);
			}
			ft_dprintf(2, "[pipe] pipe[0] = %d, pipe[1] = %d\n", pipe_fds[0], pipe_fds[1]);
			args.fds[1] = pipe_fds[1];
			if (!exec_cmd(&args, term))
			{
				ft_dprintf(2, "[pipe][exec] failed!\n");
				return (false);
			}
			if (false && !close(pipe_fds[1]))
			{
				ft_dprintf(2, "[pipe][error] close(%d) returned '-1'!\n", pipe_fds[1]); // DEBUG
				ft_dprintf(2, "[errno] %d: %s\n", errno, strerror(errno)); // DEBUG
				return (false);
			}
			ft_dprintf(2, "[pipe][next] index: %d, next: %p\n", index, curr->next);
			return (exec_pipe_cmd(index == 1 ? curr->next : curr, term, pipe_fds[0], 1));
		}
		else
		{
			ft_dprintf(2, "[pipe][last] index: %d, next: %p\n", index, curr->next);
			return (exec_cmd(&args, term)); // TODO: Differentiate between errors and status
		}
	}
	dprintf(2, "PIPE DIDN'T EXECUTED\n");
	return (true);
}
