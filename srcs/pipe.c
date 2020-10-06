/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 17:24:51 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 01:53:25 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static bool			select_fd(t_pipe2 **p)
{
	if ((*p)->in[0] \
			&& (dup2((*p)->in[0], STDIN_FILENO) < 0 \
			|| close((*p)->in[0]) < 0))
		return (false);
	if ((*p)->fd[1] != STDOUT_FILENO \
			&& (dup2((*p)->fd[1], STDOUT_FILENO) < 0 \
			|| close((*p)->fd[1]) < 0))
		return (false);
	return (true);
}

bool			is_builtin_pipe(char *name, t_pipe2 **p)
{
		return ((!ft_strncmp(name, "echo", 5) || !ft_strncmp(name, "cd", 3) \
				|| !ft_strncmp(name, "pwd", 4) || !ft_strncmp(name, "export", 7) \
				|| !ft_strncmp(name, "unset", 6) || !ft_strncmp(name, "env", 4) \
				|| !ft_strncmp(name, "exit", 5)) && select_fd(p));
}

static bool			execute_child_process(t_pipe2 *p, char **av, uint32_t ac, t_term *term)
{
	const t_builtin	builtin = builtin_get(av[0]);
	char			*execution_path;
	char			**envp;
	t_builtin_args	args;

	execution_path = NULL;
	envp = NULL;
	if (builtin)
	{
		args.ac = ac;
		args.av = av;
		args.t = term;
		redir_fds(args.fds, NULL, NONE);
		if (!(select_fd(&p)))
			ft_dprintf(2, "[exec][pipe] Error with 'select_fd'!");
		builtin(&args);
	}
	else
	{
		if (!(get_path_and_envp(&execution_path, &envp, *av, term)))
			return (false);
		if (!(term->pid = fork()))
		{
			if (!(select_fd(&p)))
				exit (1);
			term->st = execve(execution_path, av, envp);
			ft_printf("minishell: %s: command not found\n", av[0]);
			exit(0); // need that but in other format, exit fork while execve fails
			return (false);
		}
		else if (term->pid < 0)
			return (!(term->st = 127));
		while (waitpid(term->pid, NULL, 0) < 0)
			;
		term->pid = 0;
		free(execution_path);
		free(envp);
	}
	return (true);
}

static bool			execute_cmd_or_redirection(t_bst *curr, t_term *term)
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
	return (true);
}

bool				execute_pipe_cmd(t_bst *curr, t_term *term)
{
	t_pipe2			p;
	bool			once;

	p.in[0] = 0;
	once = true;

	while (curr)
	{
		pipe(p.fd);
		if (!execute_child_process(&p, curr->av[once ? 0 : 1], curr->ac[once ? 0 : 1], term) \
				|| (close (p.fd[1]) < 0))
			return (false);
		p.in[0] = p.fd[0];
		if (!once)
			curr = curr->next;
		if (!curr->next || (curr->next && !(curr->next->operator & PIPE)))
			break ;
		once = false;
	}
	if (p.in[0] != 0 && dup2(p.in[0], STDIN_FILENO) < 0)
		return (false);
	/* Seems work for:
			cmd | cmd,
			cmd | cmd | cmd,
			cmd | cmd | cmd | cmd,
			cmd | cmd > file and
			cmd | cmd | cmd > file

		The previous while will loop until the last cmd, this last cmd will
				be executed or redirected bellow
	*/	
	return (execute_cmd_or_redirection(curr, term));
}
