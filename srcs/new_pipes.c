/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 18:07:45 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 22:32:48 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static bool		select_fd(t_pipe2* p)
{
	// to do it always, save the output for redirect it later to stdin
	if (dup2(p->fd[WRITE], STDOUT_FILENO) < 0)
		return (false);
	// do it only if there are more than 1 pipe, redirect the previous output to sdtin
	if (p->status == WRITE \
			&& (dup2(STDIN_FILENO, p->fd[READ])) < 0)
		return (false);
	return (!close(p->status == READ ? p->fd[WRITE] : p->fd[READ]));
}

void				exec_and_join_pipes(const char *filepath, char **av_ep[2], t_term *t, t_pipe2 *p)
{
	// TODO: Error handling (WIFEXITED ...)
	ft_dprintf(2, "[exec][path] '%s'\n", filepath);
	if (!(t->pid = fork()))
	{
		select_fd(p);
		t->st = execve(filepath, av_ep[0], av_ep[1]);
		ft_dprintf(2, "minishell: %s: execve returned '%d'!\n", av_ep[0][0], t->st);
		exit(EXIT_FAILURE);
		return ;
	}
	else if (t->pid < 0)
	{
		t->st = 127;
		return ;
	}
	while (waitpid(t->pid, NULL, 0) <= 0)
		;
	t->pid = 0;
}

static bool		execute_child_process(t_pipe2* p, int32_t ac, char** av ,
		t_term* term)
{
	const t_builtin builtin = builtin_get(av[0]);
	char*			execution_path;
	char**			envp;
	t_builtin_args	args;
	char			**av_ep[2];

	execution_path = NULL;
	envp = NULL;
	ft_dprintf(2, "[PIPES CHILD EXEC][command name is: %s]\n", av[0]);
	if (builtin)
	{
		args.ac = ac;
		args.av = av;
		args.t = term;
		if (!redir_fds(args.fds, NULL, NONE) || !select_fd(p))
			ft_dprintf(2, "[exec][pipe] Error with 'select_fd'!");
		builtin(&args);
		close_fds(args.fds);
	}
	else
	{
		if (!get_path_and_envp(&execution_path, &envp, *av, term))
			return (false);
		av_ep[0] = av;
		av_ep[1] = envp;
		exec_and_join_pipes(execution_path, av_ep, term, p);
		free_ptrs_and_bst(execution_path, envp, NULL);
	}
	return (!close(p->status == READ ? p->fd[WRITE] : p->fd[READ]));
}
static bool			execute_cmd_or_redirection(t_pipe2 *p, t_bst *curr, t_term *term)
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
	{
		curr->av[0] = curr->av[1];
		execute_simple_cmd(curr, term);
	}
	return (!close(p->fd[READ]));
}

bool				execute_pipe_cmd(t_bst *curr, t_term *term)
{
	t_pipe2			p;
	bool			do_while;

	do_while = true;
	p.status = READ;
	int32_t debug_it = 1;
	while (42)
	{
		ft_dprintf(2, "[PIPES EXEC][LOOPS %d TIMES]\n", debug_it++);
		if (pipe(p.fd) < 0)
			return (false);
		if (!execute_child_process(&p, curr->ac[do_while ? 0 : 1], \
				curr->av[do_while ? 0 : 1], term))
			return (false);
		p.status = p.status == READ ? WRITE : READ; // should it be just 1 the first time read and the rest write ?
		if (do_while == false)
			curr = curr->next;
		// break if there's only 1 pipe or before the last if there are more
		if (!curr->next || (curr->next && (!(curr->next->operator & PIPE))))
			break ;
		do_while = false;
	}
	ft_dprintf(2, "THIS LINE SHOULD BE ALWAYS EXECUTED\n");
	ft_dprintf(2, "[PIPE EXEC][NAME OF LAST IS : %s]\n", curr->av[1][0]);
	// then redirect the previous output to stdin
	if (dup2(STDIN_FILENO, p.fd[READ]) < 0 || close(p.fd[WRITE]) < 0)
		return (false);
	// finally execute (now the previous output is in stdin)
	return (execute_cmd_or_redirection(&p, curr, term));
}

/* Notes:
	- Nothing works per the moment
	- No-builtins pipe are better than builtins pipes
	- No-builtins pipe loop correctly put have no output
	- Builtins pipe doesn't loop`correctly
*/