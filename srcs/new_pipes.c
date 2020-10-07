/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 18:07:45 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 20:50:52 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

bool					redir_fds(int* fds, const char* filepath, t_operator_t op)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (op & REDIR_GR \
		&& (fds[1] = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, umask)) < 0)
		return (false);
	else if (op & REDIR_DG \
		&& (fds[1] = open(filepath, O_WRONLY | O_CREAT | O_APPEND, umask)) < 0)
		return (false);
	else if (op & REDIR_LE \
		&& (fds[0] = open(filepath, O_RDONLY)) < 0)
		return (false);
	else
		return (!(fds[0] = 0) && (fds[1] = 1));
	return (fds[0] >= 0 && fds[1] >= 0);
}

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

static bool		execute_child_process(t_pipe2* p, int32_t ac, char** av ,
		t_term* term)
{
	const t_builtin builtin = builtin_get(av[0]);
	char*			execution_path;
	char**			envp;
	t_builtin_args	args;

	execution_path = NULL;
	envp = NULL;
	ft_dprintf(2, "[PIPES CHILD EXEC][command name is: %s]\n", av[0]);
	if (builtin)
	{
		args = (t_builtin_args){.ac=ac, .av=av, .t=term};
		if (!redir_fds(args.fds, NULL, NONE) || !select_fd(p))
			ft_dprintf(2, "[exec][pipe] Error with 'select_fd'!");
		builtin(&args);
	}
	else
	{
		if (!get_path_and_envp(&execution_path, &envp, *av, term))
			return (false);
		if (!(term->pid = fork()) && select_fd(p))
		{
			term->st = execve(execution_path, av, envp);
			ft_printf("minishell: %s: command not found\n", av[0]);
			exit(EXIT_FAILURE);
			return (false);
		}
		else if (term->pid < 0)
			return (!(term->pid = 0));
		else
			return (false);
		while (waitpid(term->pid, NULL, 0) < 0)
			;
		free(execution_path);
		free(envp);
		term->pid = 0;
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
		ft_dprintf(2, "[PIPES EXEC][LOOPS %d TIMES]\n", debug_it);
		if (pipe(p.fd) < 0)
			return (false);
		if (!execute_child_process(&p, curr->ac[do_while ? 0 : 1], \
				curr->av[do_while ? 0 : 1], term))
			return (false);
		p.status = p.status == READ ? WRITE : READ;
		if (do_while == false)
			curr = curr->next;
		// break if there's only 1 pipe or before the last if there are more
		if (!curr->next || (curr->next && (!(curr->next->operator & PIPE))))
			break ;
		do_while = false;
	}
	ft_dprintf(2, "[PIPE EXEC][NAME OF LAST IS : %s]\n", curr->av[0][0]);
	// then redirect the previous output to stdin
	if (dup2(STDIN_FILENO, p.fd[READ]) < 0 || close(p.fd[WRITE]) < 0)
		return (false);
	
	// finally execute (now the previous output is in stdin)
	return (execute_cmd_or_redirection(&p, curr, term));
}