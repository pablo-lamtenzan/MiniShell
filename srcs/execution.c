/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 21:44:08 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

bool	redir_fds(int *fds, const char *filepath, t_operator_t op)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	if (op & REDIR_GR)
		fds[1] = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, umask);
	else if (op & REDIR_DG)
		fds[1] = open(filepath, O_WRONLY | O_CREAT | O_APPEND, umask);
	else if (op & REDIR_LE)
		fds[0] = open(filepath, O_RDONLY);
	else
		return (!(fds[0] = 0) && (fds[1] = 1));
	return (fds[0] >= 0 && fds[1] >= 0);
}

void	close_fds(int *fds)
{
	if (fds[0] != 0)
		close(fds[0]);
	if (fds[1] != 1)
		close(fds[1]);
}

bool				exec_builtin(t_builtin_args *args)
{
	const t_builtin	builtin_fn = builtin_get(args->av[0]);

	if (builtin_fn)
	{
		builtin_fn(args);
		return (true);
	}
	return (false);
}

bool				get_path_and_envp(char **filepath, char***envp, char *cmd_name, t_term *term)
{
	t_map	*path;

	if (!(path = map_get(term->env, "PATH"))
	|| !path->value
	|| !(*filepath = path_get(cmd_name, path->value)))
	{
		term->st = 127;
		ft_dprintf(2, "[exec][path] cannot find '%s'\n", cmd_name);
		return (true);
	}
	if (!(*envp = map_export(term->env)))
	{
		ft_dprintf(2, "could not export environment!");
		free(filepath);
		return (false);
	}
	return (true);
}

/* I'm modifying data->envp, probally a bad idea TO SEE LATER*/

/* Normally witouth redirecting the fd, dont need to fork */
void				exec_and_join(const char *filepath, char **av_ep[2], t_term *t, t_bst *curr)
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

bool				execute_simple_cmd(t_bst *curr, t_term *term) // tokenize -> token -> 
{
	const t_builtin	builtin = builtin_get(curr->av[0][0]);
	char			*execution_path = NULL;
	char*			*envp = NULL;
	t_builtin_args	args;
	char**			av_ep[2];

	
	if (builtin)
	{
		if (!redir_fds(args.fds, (curr->av[1]) ? curr->av[1][0] : NULL, curr->operator))
			ft_dprintf(2, "[exec][redir] Error!\n");
		args.ac = curr->ac[0];
		args.av = curr->av[0];
		args.t = term;
		builtin(&args);
		close_fds(args.fds);
	}
	else
	{
		if (!(get_path_and_envp(&execution_path, &envp, curr->av[0][0], term)))
				return (free_bst_node(&curr));
		av_ep[0] = curr->av[0];
		av_ep[1] = envp;
		exec_and_join(execution_path, av_ep, term, NULL);
		free_ptrs_and_bst(execution_path, envp, NULL);
		// have to free curr ?
	}
	return (true);
}

bool			open_and_dup_stdio(t_bst *curr)
{
	int				fd;

	fd = -1;
	if (curr->operator & REDIR_GR)
	{
		if ((fd = open(curr->av[1][0], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIR_LE)
	{
		if ((fd = open(curr->av[1][0], O_RDONLY)) < 0 \
				|| dup2(fd, STDIN_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIR_DG)
	{
		if ((fd = open(curr->av[1][0],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	return (fd > 0 && !(close(fd) < 0));
}

bool				execute_redirections_cmd(t_bst *curr, t_term *term)
{
	const t_builtin	builtin = builtin_get(curr->av[0][0]);
	char			*execution_path = NULL;
	char			**envp = NULL;
	t_builtin_args	args;
	char**			av_ep[2];

	if (builtin)
	{
		if (!redir_fds(args.fds, (curr->av[1]) ? curr->av[1][0] : NULL, curr->operator))
			ft_dprintf(2, "[exec][redir] Error!\n");
		args.ac = curr->ac[0];
		args.av = curr->av[0];
		args.t = term;
		builtin(&args);
		close_fds(args.fds);
	}
	else
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
