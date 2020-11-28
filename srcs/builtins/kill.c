/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 00:31:08 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>
#include <signal.h>

// TODO: Cross platform compatibility for missing signals
// TODO: Test on 42 XUbuntu VM
#ifndef SIGSTKFLT
# define SIGSTKFLT -1
#endif

#ifndef SIGPWR
# define SIGPWR -1
#endif

// NORME LATER BUT THIS AS GLOBAL... fck norme
static const char*	get_signal(const char* key, int* res)
{
	static const char*	signals[31] = {
		"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
		"SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
		"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
		"SIGXFSZ", "SIGVALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
		"SIGSYS"
	};
	static const int	values[31] = {
		SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE,
		SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM,
		SIGSTKFLT, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU,
		SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
		SIGIO, SIGPWR, SIGSYS
	};
	static const char*	cvalues[31] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
		 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
		 "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
	};
	int			i;

	i = 0;
	if ((*key == 'l' || *key == 'L') && !*(key + 1))
	{
		*res = 256;
		return ("");
	}
	while (i < 31 && ft_strncmp(signals[i], key, ft_strlen(signals[i])) 
			&& ft_strncmp(&signals[i][3], key, ft_strlen(signals[i]) - 3)
			&& ft_strncmp(cvalues[i], key, ft_strlen(cvalues[i]) + 1))
		i++;
	if (i == 31 && !(*res = 0))
		return (NULL);
	*res = values[i];
	return (signals[i]);
}

static void		print_all_signals()
{
	int			i;
	int			tmp;
	const char*	cvalues[31] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
		 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
		 "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

	i = -1;
	while (++i < 31)
	{
		ft_dprintf(STDERR_FILENO, "%2d) %-9s ", i + 1, get_signal(cvalues[i], &tmp));
		if ((i + 1) % 5 == 0)
			write(STDERR_FILENO, "\n", 1);
	}
	write(STDERR_FILENO, "\n", 1);
}

void		kill_core(int signal)
{
	kill(g_session->groups->active_processes->pid, signal);
	if (!(signal >= SIGSTOP && signal <= SIGTTOU) && signal != SIGCONT)
	{
		g_session->groups->active_processes->flags |= (SIGNALED | KILLED);
		kill(g_session->groups->active_processes->pid, SIGCONT);
	}
	background_update(&g_session->groups->active_processes);
	history_session_remove_node(g_session->groups);
}

void		kill_group(t_process* leader, int signal, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;
	
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes \
				!= g_session->groups->nil)
			{
				kill_core(signal);
				g_session->groups->active_processes = \
					g_session->groups->active_processes->next;
			}
				g_session->groups->active_processes = remember_leader;
			break ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

int		handle_current(t_process*** target, const char* jobspec)
{
	t_group*	remember;

	remember = g_session->groups;
	ft_dprintf(2, )
	if (!ft_strncmp(jobspec, "%", 2) || !ft_strncmp(jobspec, "%+", 3) \
		|| !ft_strncmp(jobspec, "%", 2) || !ft_strncmp(jobspec, "%%", 3) \
		|| (g_session->hist && !g_session->hist->next \
		&& !ft_strncmp(jobspec, "%-", 3)))
	{
		while (g_session->groups != g_session->nil->prev)
		{
			*target = g_session->groups->next != g_session->nil ? \
				 &g_session->groups->next->active_processes : \
			 	&g_session->groups->active_processes;
			if ((**target)->flags & (KILLED | SIGNALED))
				g_session->groups = g_session->groups->next;
			else
				break ;
		}
		g_session->groups = remember;
		ft_dprintf(2, "TEST\n");
		return (true);
	}
	return (false);
}

int			kill_init_exeption(t_exec *args, int *signal)
{
	if (args->ac == 1 || (args->ac == 2 && args->av[1][0] == '-' \
			&& ft_strncmp("-l", args->av[1], 3) \
			&& ft_strncmp("-L", args->av[1], 3)))
	{
		ft_dprintf(STDERR_FILENO, "%s\n", \
		"kill: usage: kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]");
        return (CMD_BAD_USE);
	}
	if (!get_signal(&args->av[1][1], signal) && (*signal == 0) && args->av[1][0] == '-')
	{
		ft_dprintf(2, "minish: kill: %s: invalid signal specification\n", args->av[1]);
		return (CMD_BAD_USE);
	}
	if (args->av[1][0] == '%')
		*signal = 0;
	return (42);
}

int			kill_jobspec(t_exec *args, int vars[5])
{
	t_process** target;

	target = NULL;
	vars[4] = handle_current(&target, args->av[(vars[0] ? 1 : 0) + vars[1]]);
	if (!vars[4] && !(target = jobspec_parser(args->ac, &args->av[(vars[0] ? 1 : 0) + vars[1]], NULL)))
	{
		vars[4] = false;
		vars[3] = CMD_BAD_USE;
		if (args->av[(vars[0] ? 2 : 1) + vars[1]][0] == '%')
			ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[(vars[0] ? 2 : 1) + vars[1]]);
		else if (is_string_digit(args->av[(vars[0] ? 2 : 1) + vars[1]]))
			ft_dprintf(2, "minish: kill: (%s) - No such process\n", args->av[(vars[0] ? 2 : 1) + vars[1]]);
		else
			ft_dprintf(2, "minish: kill: %s: arguments must be process or job IDs\n", args->av[(vars[0] ? 2 : 1) + vars[1]]);
	}
	else if (vars[4])
	{
		if (!target || ((*target)->flags & (SIGNALED | KILLED) && (vars[3] = CMD_NOT_FOUND)))
			ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[(vars[0] ? 2 : 1) + vars[1]]);
		else
			kill_group(*target, vars[0] ? vars[0] : SIGTERM, g_session->groups);
	}
	return (42);
}

void			kill_print_signal(t_exec *args, int vars[5])
{
	char*	numeric;

	if ((numeric = (char*)get_signal(args->av[2 + vars[1]], &vars[2])))
		ft_dprintf(args->fds[1], "%s\n", numeric);
	else
	{
		vars[3] = CMD_BAD_USE;
		ft_dprintf(2, "minish: kill: %s: invalid signal specification\n", args->av[2 + vars[1]]);
	}
}

// DO TO: find: ft_dprintf(STDERR_FILENO, "%s\n", "minish: kill: COT: invalid signal specification");
	// in true kill

/*
int			ft_kill(t_exec* args)
{
	int		signal;
	int		i;
	t_process** target;
	char*	numeric;
	int		tmp;
	int		ret;
	bool 	flag = true;

	signal = 0;
	i = -1;
	target = NULL;
	if ((i = kill_init_execption(args, &signal)) != 42)
		return (i);
	if (!(signal == 256 && args->ac == 2))
	{
		while (++i < args->ac - (signal ? 2 : 1))
		{
			ret = SUCCESS;
			flag = true;
			if (signal == 256)
			{
				// catch -l here
				if ((numeric = (char*)get_signal(args->av[2 + i], &tmp)))
					ft_dprintf(args->fds[1], "%s\n", numeric);
				else
				{
					ret = CMD_BAD_USE;
					ft_dprintf(2, "minish: kill: %s: invalid signal specification\n", args->av[2 + i]);
				}
				continue ;
			}
			if ((flag = handle_current(&target, args->av[(signal ? 1 : 0) + i])))
				;
			if (!flag && !(target = jobspec_parser(args->ac, &args->av[(signal ? 1 : 0) + i], NULL)))
			{
				flag = false;
				ret = CMD_BAD_USE;
				if (args->av[(signal ? 2 : 1) + i][0] == '%')
					ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[(signal ? 2 : 1) + i]);
				else if (is_string_digit(args->av[(signal ? 2 : 1) + i]))
					ft_dprintf(2, "minish: kill: (%s) - No such process\n", args->av[(signal ? 2 : 1) + i]);
				else
					ft_dprintf(2, "minish: kill: %s: arguments must be process or job IDs\n", args->av[(signal ? 2 : 1) + i]);
			}
			else if (flag)
			{
				if (!target || ((*target)->flags & (SIGNALED | KILLED) && (ret = CMD_NOT_FOUND)))
					ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[(signal ? 2 : 1) + i]);
				else
					kill_group(*target, signal ? signal : SIGTERM, g_session->groups);
			}
		}
		return (ret);
	}
	if (signal == 256)
		print_all_signals();		
	return (SUCCESS);
}
*/


int			ft_kill(t_exec* args)
{
	int		vars[5];
	int		exept;
	// vars 0 -> signal
	// vars 1 -> i
	// vars 2 -> tmp
	// vars 3 -> ret
	// vars 4 -> flag

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
	if (vars[0] == 256)
		print_all_signals();		
	return (SUCCESS);
}
