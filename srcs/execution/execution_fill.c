/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:05:45 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 01:34:19 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>
#include <string.h>

int     	handle_wstatus(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
		ft_dprintf(2, "%s\n", strsignal(wstatus));
	return (wstatus);
}


int			execute_child(t_exec* info, t_term* term)
{
	int		wstatus;

	if (!(term->pid = fork()))
	{
		if (dup_stdio(info->fds))
		{
			wstatus = execve(info->execution_path, info->av, info->ep);
			ft_dprintf(2, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		}
		exit(EXIT_FAILURE);
	}
	else if (term->pid < 0)
		return (errno);
	while (waitpid(term->pid, &wstatus, 0) <= 0)
		;
	return (handle_wstatus(wstatus));
}

bool		build_execve_args(t_exec** info, t_term* term)
{

	if (!((*info)->execution_path = path_get((*info)->av[0], env_get(term->env, "PATH", 4))))
		return (!(term->st = 127));
	if (!((*info)->ep = (char*const*)env_export(term->env)))
	{
		free((void*)(*info)->execution_path);
		return (false);
	}
	return (true);
}

void		destroy_execve_args(t_exec* info)
{
	char**	aux;
	int		i;

	aux = (char**)info->ep;
	i = 0;
	while (aux && aux[i++])
		free(aux);
	free(aux);
	free((void*)info->execution_path);
}

int			matrix_height(char*const* matrix)
{
	const char** it;

	it = (const char**)matrix;
	while (*it)
		it++;
	return ((char*const*)it - matrix);
}
