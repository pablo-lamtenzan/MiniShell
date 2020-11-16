/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 13:29:11 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>
#include <errors.h>
#include <errno.h>

static int	ft_fork(t_exec* info, t_term* term) 
{
	int		child_st;
	child_st = fork();
	term->session->processes[term->session->processes[MANAGE].pid].pid;
	if (child_st == 0)
	{
		// for the moment overwrite the last
		if (term->session->processes[MANAGE].pid < PROCESSES_MAX)
			term->session->processes[MANAGE].pid++;
		term->session->processes->data = info->av;
	}
	return (child_st);
}

int		execute_child(t_exec* info, t_term* term)
{
	int				wstatus;
	pid_t			pid;
	t_exec_status	st;

	if (!(pid = ft_fork(info, term)))
	{
		if ((st = dup_stdio(info->fds)) != SUCCESS)
			return (st);
		wstatus = execve(info->execution_path, info->av, info->ep);
		ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		return (errno);
	//while (waitpid(term->pid, &wstatus, 0) <= 0)
	//	;
	//return (handle_wstatus(wstatus, info->av));
	term->session->processes[term->session->processes[MANAGE].pid].wstatus = wstatus;
	return (wstatus);
}

t_exec_status		build_execve_args(t_exec* info, t_term* term)
{
	if (!(info->execution_path = path_get(info->av[0], env_get(term->env, "PATH", 4))))
	{
		term->st = CMD_NOT_FOUND;
		return (SUCCESS);
	}
	if (!(info->ep = (char*const*)env_export(term->env)))
	{
		free(info->execution_path);
		return (BAD_ALLOC);
	}
	return (SUCCESS);
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
