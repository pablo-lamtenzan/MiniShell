/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:59:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 00:56:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>
#include <signal.h>

/* TEST

1) -l flag witout jobspec or sigspec DONE
2) -l with wrong sigspec DONE
3) -l with good sigspec DONE
4) kill no flags wrong jobspec DONE
5) kill invalid flags with jobspec/no jobspec
6) kill empty session DONE
-------------------------------------
7) kill without sigpec a job -> Doesn't terminates the process eigher all is ok
8) kill with jobspec a job -> Depend of the signal (need investigation about this)

9) do in bash kill % 2 times with 1 process in background -> DO TO the error msg

*/


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
	ft_dprintf(2, "[GET SIGNAL][key is = \'%s\']\n", key);
	const char*	signals[31] = {
		"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
		"SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
		"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
		"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
		"SIGXFSZ", "SIGVALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
		"SIGSYS"
	};
	const int	values[31] = {
		SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE,
		SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM,
		SIGSTKFLT, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU,
		SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
		SIGIO, SIGPWR, SIGSYS
	};
	const char*	cvalues[31] = { "1", "2", "3", "4", "5", "6", "7", "8", "9",
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
	while (i < 31 && ft_strncmp(signals[i], key, ft_strlen(signals[i])) // signal name
			&& ft_strncmp(&signals[i][3], key, ft_strlen(signals[i]) - 3) // skip sig from the name
			&& ft_strncmp(cvalues[i], key, ft_strlen(cvalues[i]) + 1)) // numeric value
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

int		get_target(t_exec* args, bool sig_spec, t_term* term, t_process*** target)
{
	(void)term;
	if (!(*target = jobspec_parser(args->ac, args->av, NULL)) && !sig_spec)
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

void		kill_group(t_process* leader, int signal, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;
	
	// skip itself
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;

	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				g_session->groups->active_processes->flags |= SIGNALED;
				if (PRINT_DEBUG)
					ft_dprintf(2, "[KILL][KILL (signal=\'%d\') \'%d\']\n", signal, g_session->groups->active_processes->pid);
				kill(g_session->groups->active_processes->pid, signal);
				kill(g_session->groups->active_processes->pid, SIGCONT);
				update_background(&g_session->groups->active_processes, true /*signal == SIGCONT*/);
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			// Remove the group later
			/*if (!is_active_group(g_session->groups))
			{
				t_group*	fill = g_session->groups;
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
			}
			*/
			//else
				g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
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
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[KILL][SIGNAL FORMAT IS: %d]\n", sig_spec);
	ft_dprintf(2, "[SIGNAL IS %d]\n", signal);}
    if (signal == 256)
    { 
        if (args->ac == 2)
			print_all_signals();
		else if (signal && (test = (char*)get_signal(&args->av[2][0], &signal)))
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
		// TO DO: only the target if pid
		if ((st = get_target(args, sig_spec, term, &target)) != SUCCESS)
			return (st);
		if ((*target)->flags & RESTRICT_OP)
		{
			ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[1]);
            	return (STD_ERROR);
		}
		kill_group(*target, SIGTERM, g_session->groups);
		return (st);
	}
	if (signal)
	{
		// TO DO: only the target if pid
		if (!(target = jobspec_parser(args->ac, &args->av[1], NULL)) && !sig_spec)
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
		else if (!target && sig_spec)
		{
			ft_dprintf(2, "%s", "minish: kill: invalid signal specification\n");
			return (STD_ERROR);
		}
		if ((*target)->flags & RESTRICT_OP)
		{
			ft_dprintf(STDERR_FILENO, "minish: kill: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
		}
		kill_group(*target, signal, g_session->groups);
		return (SUCCESS); // check this ret
	}
	ft_dprintf(STDERR_FILENO, "%s\n", "minish: kill: COT: invalid signal specification");
	return (STD_ERROR);
}
