/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 18:07:45 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/08 22:28:22 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static bool		exec_and_join_pipes(const char* filepath, char** av_ep[2],
		t_term *t, int32_t* fds)
{
	// TODO: Error handling (WIFEXITED ...)
	ft_dprintf(2, "[exec][path] '%s'\n", filepath);
	if (!(t->pid = fork()) && dup_stdio(fds))
	{
		t->st = execve(filepath, av_ep[0], av_ep[1]);
		ft_dprintf(2, "minishell: %s: execve returned '%d'!\n", av_ep[0][0], \
			t->st);
		exit(EXIT_FAILURE);
	}
	else if (t->pid < 0)
		return (!(t->st = 127));
	while (!waitpid(t->pid, NULL, 0))
		;
	return (!(t->pid = 0));
}

static bool		execute_child(int fds[3], int32_t ac, char** av, t_term* term)
{
	
	char*					execution_path;
	char**					envp;
	const t_builtin_exec 	b = (t_builtin_exec){.ac=ac, .av=av, .fds=fds};
	char**					av_ep[2];

	execution_path = NULL;
	envp = NULL;
	if (!if_builtin_exec(b, term, fds))
	{
		if (!get_path_and_envp(&execution_path, &envp, *av, term))
			return (false);
		av_ep[0] = av;
		av_ep[1] = envp;
		exec_and_join_pipes(execution_path, av_ep, term, fds);
		free_ptrs_and_bst(execution_path, envp, NULL);
	}
	return (true);
}

static bool		execute_cmd_or_redirection(int32_t fds[3], t_bst *curr, t_term *term)
{
	if (curr->next && (curr->next->operator & REDIR_GR \
			|| curr->next->operator & REDIR_DG \
			|| curr->next->operator & REDIR_LE))
	{
		curr->av[0] = curr->av[1];
		curr->av[1] = curr->next->av[1];
		execute_redirections_cmd(curr, term);
	}
	else
		execute_child(fds, curr->ac[1], curr->av[1], term);
	return (true);
}

bool			execute_pipes_cmd(int in_fd, t_bst* curr, t_term* term)
{
	int32_t		fds[3];
	int32_t		fildes[2];

	if (curr)
	{
		fds[0] = in_fd == -1 ? STDIN_FILENO : in_fd;
		fds[1] = STDOUT_FILENO;
		fds[2] = STDERR_FILENO;
		if (in_fd == -1 || (curr->next && curr->next->operator & PIPE))
		{
			if (pipe(fildes) < 0)
				return (false);
			fds[1] = fildes[WRITE];
			if (!execute_child(fds, curr->ac[EXEPT], curr->av[EXEPT], term) \
					|| close(fildes[WRITE]) < 0)
				return (false);
			execute_pipes_cmd(fildes[READ], NEXT_NODE, term);
		}
		else
			return (execute_cmd_or_redirection(fds, curr, term));
	}
	else
		;
	return (true);
}