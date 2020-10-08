/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/08 22:50:22 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static bool		if_builtin_exec2(const t_builtin_exec b, t_bst* curr, t_term* term)
{
	const t_builtin builtin = builtin_get(b.av[0]);
	t_builtin_args	args;

	if (builtin)
	{
		if (!redir_fds(args.fds, (curr->av[1]) ? curr->av[1][0] : NULL, curr->operator))
			ft_dprintf(2, "[exec][redir] Error!\n");
		args.ac = curr->ac[0];
		args.av = curr->av[0];
		args.t = term;
		term->st = builtin(&args);
		close_fds(args.fds);
	}
	return (builtin);
}

static bool			exec_and_join(const char* filepath, char** av_ep[2], t_term* t, t_bst* curr)
{
	// TODO: Error handling (WIFEXITED ...)
	ft_dprintf(2, "[exec][path] '%s'\n", filepath);
	if (!(t->pid = fork()))
	{
		if (curr)
			open_and_dup_stdio(curr);
		t->st = execve(filepath, av_ep[0], av_ep[1]);
		ft_dprintf(2, "minishell: %s: execve returned '%d'!\n", av_ep[0][0], t->st);
		exit(EXIT_FAILURE);
		return (false);
	}
	else if (t->pid < 0)
		return (!(t->st = 127));
	while (waitpid(t->pid, NULL, 0) <= 0)
		;
	return (!(t->pid = 0));
}

bool				execute_simple_cmd(t_bst* curr, t_term* term)
{
	char*			execution_path = NULL;
	char**			envp = NULL;
	t_builtin_exec	b;
	char**			av_ep[2];

	b.ac = curr->ac[0];
	b.av = curr->av[0];
	b.fds = NULL;
	if (!if_builtin_exec2(b, curr, term))
	{
		if (!(get_path_and_envp(&execution_path, &envp, curr->av[0][0], term)))
				return (free_bst_node(&curr));
		av_ep[0] = curr->av[0];
		av_ep[1] = envp;
		exec_and_join(execution_path, av_ep, term, NULL);
		free_ptrs_and_bst(execution_path, envp, NULL);
		// have i to free curr ?
	}
	return (true);
}

bool			open_and_dup_stdio(t_bst* curr)
{
	int				fd;

	fd = -1;
	if (curr->operator & REDIR_GR \
			&& ((fd = open(curr->av[1][0], O_WRONLY | O_CREAT | O_TRUNC, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
			|| dup2(fd, STDOUT_FILENO) < 0))
		return (false);
	else if (curr->operator & REDIR_LE \
			&& ((fd = open(curr->av[1][0], O_RDONLY)) < 0 \
			|| dup2(fd, STDIN_FILENO) < 0))
		return (false);
	else if (curr->operator & REDIR_DG \
			&& ((fd = open(curr->av[1][0],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
			|| dup2(fd, STDOUT_FILENO) < 0))
		return (false);
	return (fd > 0 && !(close(fd) < 0));
}

bool				execute_redirections_cmd(t_bst* curr, t_term* term)
{
	char*			execution_path = NULL;
	char**			envp = NULL;
	t_builtin_exec	b;
	char**			av_ep[2];

	b.ac = curr->ac[0];
	b.av = curr->av[0];
	b.fds = NULL;
	if (!if_builtin_exec2(b, curr, term))
	{
		if (!(get_path_and_envp(&execution_path, &envp, *curr->av[0], term)))
			return (free_bst_node(&curr));
		av_ep[0] = curr->av[0];
		av_ep[1] = envp;
		exec_and_join(execution_path, av_ep, term, curr);
		free_ptrs_and_bst((void*)execution_path, (void*)envp, NULL);
		// have to free curr ?
	}
	return (true);
}
