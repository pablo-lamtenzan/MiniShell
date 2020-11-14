/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 08:09:46 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>

int			execute_child(t_exec* info, t_term* term)
{
	int		wstatus;

	if (!(term->processes[term->processes[MANAGE].pid].pid = fork()))
	{
		term->processes[MANAGE].pid++;
		term->processes[term->processes[MANAGE].pid].data = info->av;
		if (dup_stdio(info->fds))
		{
			wstatus = execve(info->execution_path, info->av, info->ep);
			ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		}
		exit(EXIT_FAILURE);
	}
	else if (term->processes[term->processes[MANAGE].pid].pid < 0)
		return (errno);
	//while (waitpid(term->pid, &wstatus, 0) <= 0)
	//	;
	//return (handle_wstatus(wstatus, info->av));
	return (term->processes[term->processes[MANAGE].pid].wstatus = wstatus);
}

bool		build_execve_args(t_exec* info, t_term* term)
{
	ft_dprintf(2, "Goes into build_execve_args\n");
	if (!(info->execution_path = path_get(info->av[0], env_get(term->env, "PATH", 4))))
		return (!(term->st = 127));
	ft_dprintf(2, "[EXECUTION PATH][%s]\n", info->execution_path);
	if (!(info->ep = (char*const*)env_export(term->env)))
	{
		free(info->execution_path);
		return (false);
	}
//	ft_dprintf(2, "[ENV][%p]\n", info->ep);
	return (true);
}

void		destroy_execve_args(t_exec *info)
{
	free((char **)info->ep);
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
