/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 21:35:55 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signal.h>

// TODO: Cross platform compatibility for missing signals
// TODO: Test on 42 XUbuntu VM
#ifndef SIGSTKFLT
# define SIGSTKFLT -1
#endif

#ifndef SIGPWR
# define SIGPWR -1
#endif

const static char*	get_signal(const char* key, int* res)
{
	const char*	signals[32] = {
		"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
		"SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
		"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
		"SIGXCPU", "SIGXFSZ", "SIGVALRM", "SIGPROF", "SIGWINCH", "SIGIO",
		"SIGPWR", "SIGSYS"
	};
	const int	values[32] = {
		SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE,
		SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM,
		SIGSTKFLT, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU,
		SIGURG, SIGXCPU, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
		SIGIO, SIGPWR, SIGSYS
	};
	const char*	cvalues[32] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
		 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21",
		 "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", ""
	};
	int			i;

	i = 0;
	if ((*key == 'l' || *key == 'L') && !*(key + 1))
	{
		*res = 256;
		return ("");
	}
	while (i < 31 && ft_strncmp(signals[i], key, ft_strlen(signals[i])) // signal name
			&& ft_strncmp(&signals[i][3], key, ft_strlen(signals[i]) - 3) // skip sig from the name
			&& ft_strncmp(cvalues[i], key, ft_strlen(cvalues[i]))) // numeric value
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
		ft_dprintf(STDERR_FILENO, "%2d) %-7s ", i + 1, get_signal(cvalues[i], &tmp));
		if ((i + 1) % 5 == 0)
			write(STDERR_FILENO, "\n", 1);
	}
	write(STDERR_FILENO, "\n", 1);
}

int		get_target(t_exec* args, bool sig_spec, t_term* term, t_process*** target)
{

	if (!(*target = jobspec_parser(term->session, args->ac, args->av, NULL)) && !sig_spec)
	{
		if (args->av[1][0] == '%')
			ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[1]);
		else if (is_string_digit(args->av[1]))
			ft_dprintf(2, "minish: kill: (%s) - No such process\n", args->av[1]);
		else
			ft_dprintf(2, "minish: kill: %s: arguments must be process or job IDs\n", args->av[1]);
		return (STD_ERROR);
	}
	else if (sig_spec)
	{
		ft_dprintf(2, "%s", "minish: kill: invalid signal specification\n");
		return (STD_ERROR);
	}
	return (SUCCESS);
}

/*
kill [-s sigspec] [-n signum] [sigspec] jobspec or pid
kill -l|-L [exit_status]:
 -> Send a signal (sigspec or signum) to the process (job or pid)
 -> SIG prefix is optional
 -> If sigspec and signum are not present: SIGTERM is used
 -> '-l' option lists the signal names
 -> '-l' without option list all avalable options (returns 0)
 -> '-L' is equivalent to -1?? (returns 0)
 -> Returns 0 if at least 1 siganl was succefully sent
 -> Returns !0 if an error occurs ir invalid option
*/

void		kill_group(t_session* session, t_process* leader, int signal, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = session->groups;
	
	// skip itself
	if (session->groups == itself)
		session->groups = session->groups->next;

	while (session->groups != session->nil)
	{
		if (session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				ft_dprintf(2, "[KILL][KILL (signal=\'%d\') \'%d\']\n", signal, session->groups->active_processes->pid);
				kill(session->groups->active_processes->pid, signal);
				update_background(session, &session->groups->active_processes, signal == SIGCONT); // TO DO: more execptions
				session->groups->active_processes = session->groups->active_processes->next;
			}
			if (!is_active_group(session->groups))
			{
				t_group*	fill = session->groups;
				session->groups->prev->next = session->groups->next;
				session->groups->next->prev = session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				session->groups->active_processes = remember_leader;
			session->groups = remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = remember;
}

int     	ft_kill(t_exec* args, t_term* term)
{
    int			signal;
	int			st;
	char*		test;
	bool		sig_spec;
	t_process**	target;

	signal = -1;
	test = NULL;
	st = 0;
	sig_spec = false;
    if (args->ac <= 1)
    {
		// print this if the signals is present too
        ft_dprintf(STDERR_FILENO, "%s\n", "kill: usage: kill [-s sigspec | -n signum | -sigspec] pid | jobspec ... or kill -l [sigspec]");
        return (CMD_BAD_USE);
    }
	if (args->av[1][0] == '-' && (sig_spec = true))
		get_signal(&args->av[1][1], &signal); // return 256 with -l
	ft_dprintf(2, "[KILL][SIGNAL FORMAT IS: %d]\n", sig_spec);
	ft_dprintf(2, "[SIGNAL IS %d]\n", signal);
    if (signal == 256)
    { 
        if (args->ac == 2)
			print_all_signals();
		else if (signal && (test = (char*)get_signal(&args->av[2][1], &signal)))
			ft_dprintf(STDERR_FILENO, "%d\n", signal);
		else if (!test)
		{
			ft_dprintf(2, "minish: kill: %s: invalid signal specification\n", args->av[2]);
			return (STD_ERROR);
		}
		return (SUCCESS);
    }
	if (args->ac < 3)
	{
		// TO DO: all elems in groups for any jobspec
		// TO DO: only the target if pid
		if ((st = get_target(args, sig_spec, term, &target)) != SUCCESS)
			return (st);
		kill_group(term->session, *target, SIGTERM, term->session->groups);
		return (st);
	}
	if (signal)
	{
		// TO DO: all elems in groups for any jobspec
		// TO DO: only the target if pid
		if (!(target = jobspec_parser(term->session, args->ac, &args->av[1], NULL)) && !sig_spec)
		{
			// CANT get target here for the comment (av[2] istead of av[1])
			if (args->av[2][0] == '%')
				ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[2]);
			else if (is_string_digit(args->av[2]))
				ft_dprintf(2, "minish: kill: (%s) - No such process\n", args->av[2]);
			else
				ft_dprintf(2, "minish: kill: %s: arguments must be process or job IDs\n", args->av[2]);
			return (STD_ERROR);
		}
		else if (!sig_spec)
		{
			ft_dprintf(2, "%s", "minish: kill: invalid signal specification\n");
			return (STD_ERROR);
		}
		kill_group(term->session, *target, signal, term->session->groups);
		return (SUCCESS); // check this ret
	}
	ft_dprintf(STDERR_FILENO, "%s\n", "minish: kill: COT: invalid signal specification");
	return (STD_ERROR);

}
