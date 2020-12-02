/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 14:47:29 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>
#include <signals.h>

// TODO: Cross platform compatibility for missing signals
// TODO: Test on 42 XUbuntu VM



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
			ft_dprintf(STDERR_FILENO, "%s: kill: %s: no such job\n", \
			g_session.name, args->av[(vars[0] ? 2 : 1) + vars[1]]);
		else
			kill_group(*target, vars[0] ? vars[0] : SIGTERM, \
			g_session.groups);
	}
	return (42);
}

// DO TO: find: ft_dprintf(STDERR_FILENO, "%s\n", "minish: kill: COT: invalid signal specification");
	// in true kill

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

int				kill_parse(t_exec *args, int *signal)
{
	if (args->ac == 1 || (args->ac == 2 && args->av[1][0] == '-') && !(args->av[1][1] ))
}


# define	B_KILL_OPTS	"lsn"
enum			e_opt
{
	B_KILL_OSIG = 0,
	B_KILL_ONUM = 1,
	B_KILL_OLST = 2
};

static int		get_opt(t_signal **signal, t_exec *args)
{
	const char	*arg = args->av[1];
	enum e_opt	opt;

	*signal = NULL;
	if (*arg != '-' || (opt = ft_strpos(B_KILL_OPTS, *++arg)) == -1 || arg[1])
		opt = 0;
	else if (*arg == '-')
		opt = -1;
	return (opt);
}

static int		list_signals()
{
	t_list		*curr;
	t_signal	*sig;
	int			i;

	curr = g_session.signals;
	i = 1;
	while (curr)
	{
		sig = (t_signal*)curr->content;
		ft_dprintf(STDERR_FILENO, "%2d) %-9s ", sig->value, sig->name);
		if (i % 5 == 0)
			write(STDERR_FILENO, "\n", 1);
		i++;
	}
	return (0);
}

static int		print_signals(char *const *args)
{
	t_signal	*sig;
	int			value;
	int			err;

	err = 0;
	while (*args)
	{
		if (ft_isdigit(**args))
		{
			if ((sig = signal_getn(g_session.signals, ft_atoi(*args))))
				ft_dprintf(STDERR_FILENO, "%s\n", sig->name);
		}
		else if ((sig = signal_gets(g_session.signals, *args)))
			ft_dprintf(STDERR_FILENO, "%u", sig->value);
		if (!sig)
		{
			err |= 1;
			ft_dprintf(STDERR_FILENO,
				"%s: kill: %s: invalid signal specification\n", g_session.name,
				*args);
		}
		args++;
	}
	return (err);
}

int				b_kill(t_exec *args)
{
	int	opt;
	int	i;

	if (args->ac <= 1 || (opt = get_opt(&args->av[1])) == -1)
	{
		ft_dprintf(STDERR_FILENO, "kill: usage: kill [-s sigspec | -n signum | -sigspec]\
 pid | jobspec ... or kill -l [sigspec]\n");
		return (1);
	}
	if (opt == B_KILL_OLST)
	{
		if (args->ac == 2)
			return (list_signals());
		return (print_signals(&args->av[2]))
	}
	if (opt == B_KILL_OSIG)
	{
		if (ft_strcmp(args->av[], ))
	}
}