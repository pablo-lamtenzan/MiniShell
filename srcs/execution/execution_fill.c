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
	term->session->processes[term->session->processes[MANAGE].pid].pid = child_st;
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

	wstatus = -1; // TODO: Init
	if (!(pid = ft_fork(info, term)))
	{
		if ((st = dup_stdio(info->fds)) != SUCCESS)
			return (st);
		wstatus = execve(info->execution_path, info->av, info->ep);
		ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		ft_dprintf(2, "Fork returnd 0!\n");
		return (errno);
	}
	ft_dprintf(2, "[NEW FORK][pid=\'%d\', name=\'%s\']\n", pid, info->av[0]);
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
		return (RDR_BAD_ALLOC);
	}
	return (SUCCESS);
}

void		destroy_execve_args(t_exec *info)
{
	free((char **)info->ep);
	free((void*)info->execution_path);
}

int			matrix_height(char **matrix)
{
	char** it;

	it = matrix;
	while (*it)
		it++;
	return (it - matrix);
}
