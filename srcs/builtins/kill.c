/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:26:11 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control/jc_builtins.h>
#include <job_control/jobspec_parser.h>
#include <job_control/utils.h>
#include <signal.h>

void			kill_print_signal(t_exec *args, int vars[5])
{
	char		*numeric;

	if ((numeric = (char*)get_signal(args->av[2 + vars[1]], &vars[2])))
		ft_dprintf(args->fds[1], "%s\n", numeric);
	else
	{
		vars[3] = CMD_BAD_USE;
		ft_dprintf(STDERR_FILENO, \
			"%s: kill: %s: invalid signal specification\n", \
			g_session.name, args->av[2 + vars[1]]);
	}
}

int				kill_init_exeption(t_exec *args, int *signal)
{
	if (args->ac == 1 || (args->ac == 2 && args->av[1][0] == '-' \
			&& ft_strncmp("-l", args->av[1], 3) \
			&& ft_strncmp("-L", args->av[1], 3)))
	{
		ft_dprintf(STDERR_FILENO, "%s%s\n", \
			"kill: usage: kill [-s sigspec | -n signum | -sigspec]", \
			" pid | jobspec ... or kill -l [sigspec]");
		return (CMD_BAD_USE);
	}
	if (!get_signal(&args->av[1][1], signal) \
			&& (*signal <= 0) && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO,\
		"%s: kill: %s: invalid signal specification\n", \
			g_session.name, args->av[1]);
		return (CMD_BAD_USE);
	}
	if (args->av[1][0] == '%')
		*signal = 0;
	return (42);
}

static void		kill_jobspc_msg(t_exec *args, int *vars)
{
	vars[4] = false;
	vars[3] = CMD_BAD_USE;
	if (args->av[(vars[0] ? 2 : 1) + vars[1]][0] == '%')
		ft_dprintf(STDERR_FILENO, "%s: kill: %s: no such job\n", \
			g_session.name, args->av[(vars[0] ? 2 : 1) + vars[1]]);
	else if (is_string_digit(args->av[(vars[0] ? 2 : 1) + vars[1]]))
		ft_dprintf(STDERR_FILENO, "%s: kill: (%s) - No such process\n", \
			g_session.name, args->av[(vars[0] ? 2 : 1) + vars[1]]);
	else
		ft_dprintf(STDERR_FILENO, \
		"minish: kill: %s: arguments must be process or job IDs\n", \
		args->av[(vars[0] ? 2 : 1) + vars[1]]);
}

int				kill_jobspec(t_exec *args, int vars[5])
{
	t_process	**target;

	target = NULL;
	vars[4] = handle_current(&target, args->av[(vars[0] ? 2 : 1) + vars[1]]);
	if (!vars[4] && (vars[4] = true) \
			&& !(target = jobspec_parser(args->ac, \
			&args->av[(vars[0] ? 1 : 0) + vars[1]], NULL)))
		kill_jobspc_msg(args, vars);
	else if (vars[4])
	{
		if (!target || ((*target)->flags & (SIGNALED | KILLED) \
			&& (vars[3] = CMD_NOT_FOUND)))
			ft_dprintf(STDERR_FILENO, \
			"%s: kill: %s: no such job\n", \
			g_session.name, args->av[(vars[0] ? 2 : 1) + vars[1]]);
		else
			kill_group(*target, vars[0] ? vars[0] : SIGTERM, \
			g_session.groups);
	}
	return (42);
}

int				b_kill(t_exec *args)
{
	int			vars[5];
	int			exept;

	ft_bzero(&vars, sizeof(vars));
	vars[1] = -1;
	if ((exept = kill_init_exeption(args, &vars[0])) != 42)
		return (exept);
	if (!(vars[0] == 256 && args->ac == 2))
	{
		while (++vars[1] < args->ac - (vars[0] ? 2 : 1))
		{
			vars[3] = SUCCESS;
			vars[4] = true;
			if (vars[0] == 256)
			{
				kill_print_signal(args, vars);
				continue ;
			}
			if ((exept = kill_jobspec(args, vars)) != 42)
				return (exept);
		}
		return (vars[3]);
	}
	print_all_signals();
	return (SUCCESS);
}
