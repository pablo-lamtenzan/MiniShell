/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 08:02:34 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <errors.h>
#include <errno.h>

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
				g_session.name, info->av[0], ret);
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
	free((void*)info->file_path);
	info->file_path = NULL;
	info->exec = NULL;
}
