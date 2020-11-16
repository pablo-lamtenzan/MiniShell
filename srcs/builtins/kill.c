/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 16:59:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signal.h>

const static char*	get_signal(const char* key, int* res)
{
	const char*	signals[33] = {
		"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
		"SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
		"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
		"SIGXCPU", "SIGXFSZ", "SIGVALRM", "SIGPROF", "SIGWINCH", "SIGIO",
		"SIGPWR", "SIGSYS", "-l"
	};
	const int*	values[33] = {
		SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE,
		SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM,
		SIGSTKFLT, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU,
		SIGURG, SIGXCPU, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
		SIGIO, SIGPWR, SIGSYS, 256
	};
	const char*	cvalues[33] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
		 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
		 "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", ""
	};
	int			i;

	i = 0;
	while (i < 33 && ft_strncmp(signals[i], key, ft_strlen(signals[i])) // signal name
			&& ft_strncmp(&signals[i][3], key, ft_strlen(signals[i] - 3)) // skip sig from the name
			&& ft_strncmp(cvalues[i], key, ft_strlen(cvalues[i]))) // numeric value
		i++;
	if (i == 33 && !(*res = 0))
		return (NULL);
	*res = values[i];
	return (signals[i]);
}

static void		print_all_signals()
{
	int			i;
	int			tmp;
	const char*	cvalues[32] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
		 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
		 "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

	i = -1;
	while (++i < 32)
	{
		ft_dprintf(STDERR_FILENO, "%d) %17s", i, get_signal(cvalues[i], &tmp));
		if (i % 5)
			write(STDERR_FILENO, "\n", 1);
	}
	write(STDERR_FILENO, "\n", 1);
}

static pid_t	try_catch_pid(const char* str_pid, t_process* suspended)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (str_pid[++i])
		if (!ft_isdigit(str_pid[i]));
			return (0);
	pid = ft_atoi(str_pid);
	while (suspended)
	{
		if (pid == suspended->pid)
			return (pid);
		suspended = suspended->next;
	}
	return (0);
}

t_process**	get_process(pid_t pid, t_process* suspended)
{
	while (suspended)
	{
		if (suspended->pid == pid)
			return (&suspended);
	}
}

int     	ft_kill(t_exec* args, t_term* term)
{
    int		signal;
	pid_t	pid;

    if (args->ac < 1)
    {
        ft_dprintf(STDERR_FILENO, "%s\n", "kill: usage: kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]");
        return (CMD_BAD_USE);
    }
	get_signal(args->av[1], &signal); // return 256 with -l
    if (signal == 256)
    {
        if (args->ac == 2)
			print_all_signals();
		else if (signal)
			ft_dprintf(STDERR_FILENO, "%d\n", signal);
		return (SUCCESS);
    }
	if (signal)
	{
		if (!(pid = try_catch_pid(args->av[2], term->suspended_processes)))
		{
			ft_dprintf(STDERR_FILENO, "%s\n", "bash: kill: xcwac: arguments must be process or job IDs");
			return (STD_ERROR);
		}
		// handle if signal stop in read in or write out -> set ttin, ttou
		// use something like lsof ?
		kill(signal, pid);
		// wait for it after and remove it if exited
		while (waitpid(pid, &signal, 0) <= 0)
			;
		if (WIFEXITED(signal))
			remove_suspended_process(get_process(pid, term->suspended_processes));
		else
			update_used_pids(pid, &term->used_pids);
	}
	ft_dprintf(STDERR_FILENO, "%s\n", "bash: kill: COT: invalid signal specification");
	return (STD_ERROR);

}