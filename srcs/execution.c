/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/10 16:41:20 by chamada          ###   ########.fr       */
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

bool				exec_cmd(t_args *args, t_term *term)
{
	const t_builtin	builtin = builtin_get(args->av[0]);

	if (builtin)
		term->st = builtin(args, term);
	else if (get_path_and_envp((char **)&args->exec, (char ***)&args->ep, args->av[0], term))
		term->st = exec_and_join(args, term); // TODO: REFACTOR du turfu
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
		args.fds[0] = in_fd;
		args.fds[1] = STDOUT_FILENO;
		args.fds[2] = STDERR_FILENO;
		args.ac = curr->ac[index];
		args.av = curr->av[index];
		if (index == 0 || curr->next)
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
	return (true);
}

bool	dup_stdio(int fds[3])
{
	int i;

	i = 0;
	while (i < 3)
	{
		if (fds[i] != i)
		{
			dprintf(2, "[redir][dup] %d -> %d\n", fds[i], i);
			if (dup2(fds[i], i) < 0)
			{
				dprintf(2, "[redir][dup][error] dup2 returned '-1'!\n");
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (false);
			}
			dprintf(2, "[redir][close] fork fd '%d'...\n", fds[i]);
			if (close(fds[i]) < 0)
			{
				dprintf(2, "[redir][close][error] close(%d) returned -1!\n", fds[i]);
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (false);
			}
		}
		i++;
	}
	return (true);
}
