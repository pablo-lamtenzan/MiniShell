/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 21:58:48 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/08 22:41:33 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

bool				get_path_and_envp(char** filepath, char*** envp, char* cmd_name, t_term* term)
{
	t_map*			path;

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
bool				exec_builtin(t_builtin_args* args)
{
	const t_builtin	builtin_fn = builtin_get(args->av[0]);

	if (builtin_fn)
	{
		builtin_fn(args);
		return (true);
	}
	return (false);
}

void	close_fds(int *fds)
{
	if (fds[0] != 0)
		close(fds[0]);
	if (fds[1] != 1)
		close(fds[1]);
}

bool	redir_fds(int* fds, const char* filepath, t_operator_t op)
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
