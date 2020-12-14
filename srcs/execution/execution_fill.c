/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/12/13 02:18:48 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>
#include <errors.h>
#include <errno.h>
#include <builtins.h>
#include <path.h>

static int			ft_fork(t_exec *info)
{
	int				child_st;
	t_process		*process;

	if ((child_st = fork()) > 0)
	{
		if (!(process = process_new(child_st, 0, info->av)))
			return (-1);
		process_push_back(&g_session.groups, process);
	}
	return (child_st);
}

int					execute_child(t_exec *info)
{
	int				ret;
	pid_t			pid;
	t_exec_status	st;

	ret = 0;
	if (!(pid = ft_fork(info)))
	{
		if ((st = dup_stdio(info->fds)) != SUCCESS)
			return (st);
		ret = execve(info->file_path, info->av, info->ep);
		ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", \
				g_session.name, info->file_path, ret);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		return (errno);
	return (ret);
}

void				destroy_execve_args(t_exec *info)
{
	free((char **)info->ep);
	info->ep = NULL;
	if (g_session.flags & BUILTIN)
	{
		strs_unload((char**)info->av);
		info->av = NULL;
	}
}

bool				handle_subshell(t_executable exec, const char *name)
{
	if (g_session.flags & PIPED_CMD && (exec == &b_bg
	|| exec == &b_fg || exec == &b_kill || exec == &b_wait \
	|| exec == &b_disown || exec == &b_jobs))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: no job control\n", \
			g_session.name, name);
		return (false);
	}
	return (true);
}

t_exec_status		get_exec(t_exec *info, t_executable *exec)
{
	t_exec_status	status;

	status = SUCCESS;
	if (!(*exec = builtin_get(info->av[0])))
	{
		if (!path_get(&info->file_path, info->av[0], \
			env_get(info->session->env, "PATH", 4)))
			status = BAD_ALLOC;
		else if (!info->file_path)
		{
			g_session.st = CMD_NOT_FOUND;
			status = BAD_PATH;
		}
		else if (!(info->ep = (char*const*)env_export(info->session->env)))
			status = BAD_ALLOC;
		else
			*exec = &execute_child;
	}
	g_session.flags |= *exec != &execute_child ? BUILTIN : 0;
	return (status);
}
