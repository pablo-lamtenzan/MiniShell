/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 21:58:48 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/10 16:10:22 by chamada          ###   ########.fr       */
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
		return (false);
	}
	if (!(*envp = map_export(term->env)))
	{
		ft_dprintf(2, "[exec][env] could not export environment!");
		free(filepath);
		return (false);
	}
	return (true);
}

bool	close_fds(int *fds)
{
	int i;

	i = 0;
	while (i < 3)
	{
		if (fds[i] != i)
		{
			dprintf(2, "[redir] closing %d\n", fds[i]); // DEBUG
			if (close(fds[i]) < 0)
			{
				dprintf(2, "[redir] close returned '-1'!\n"); // DEBUG
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno)); // DEBUG
				return (false);
			}
		}
		i++;
	}
	return (true);
}

bool	redir_fds(int* fds, const char* filepath, t_operator_t op)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	fds[2] = STDERR_FILENO;
	if (op & REDIR_GR)
		fds[1] = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, umask);
	else if (op & REDIR_DG)
		fds[1] = open(filepath, O_WRONLY | O_CREAT | O_APPEND, umask);
	else if (op & REDIR_LE)
		fds[0] = open(filepath, O_RDONLY);
	else
		return (!(fds[0] = 0) && (fds[1] = 1));
	dprintf(2, "[redir][opened] '%s'!\n", filepath); // DEBUG
	dprintf(2, "[redir] fds[0] = %d, fds[1] = %d!\n", fds[0], fds[1]); // DEBUG
	return (fds[0] >= 0 && fds[1] >= 0);
}
