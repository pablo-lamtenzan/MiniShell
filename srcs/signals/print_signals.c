/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:45:15 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 03:14:33 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>
#include <sys/wait.h>
#include <signal.h>

// TO DO: buffer overflow
// TO DO: check more signals like time out



// SIGHUP Hangup // index 1
// SIGINT == \n <--- clems does it
// SIGQUIT Quit (core dumped)
// SIGILL Illegal instruction (core dumped)
// SIGTRAP Trace/breakpoint trap (core dumped)
// SIGABRT Aborted (core dumped)
// SIGBUS Bus error (core dumped)
// SIGFPE Floating point exception (core dumped)
// SIGKILL Killed
// SIGUSR1 User defined signal 1
// SIGSEGV Segmentation fault (core dumped)
// SIGUSR2 User defined signal 2
// SIGPIPE // no msg
// SIGALRM Alarm clock
// SIGTERM Terminated
// SIGSTKFLT Stack fault
// SIGCHLD // no msg
// SIGCONT // no msg
// SIGSTOP [2]+  Stopped                 ./a.out
// SIGTSTP // /n [3]+  Stopped                 ./a.out
// SIGTTIN  // \n [4]+  Stopped                 ./a.out
// SIGTTOU  // \n [5]+  Stopped                 ./a.out
// SIGURG // nothing
// SIGXCPU CPU time limit exceeded (core dumped)
// SIGXFSZ File size limit exceeded (core dumped)
// SIGVTALRM Virtual timer expired
// SIGPROF Profiling timer expired
// SIGWINCH // nothing
// SIGIO I/O possible
// SIGPWR Power failure
// SIGSYS Bad system call (core dumped)
// index 34
// SIGRTMIN Real-time signal 0
// SIGRTMIN (+ 1 to 15) Real-time signal [1 to 15]
// SIGRTMAX (- 14 to 1) Real-time signal [16 to 29]
// SIGRTMAX Real-time signal 30


/* New Version */

// SIGSTOP 127


void	print_job_args(int fd, t_process* target)
{
	int i;

	i = -1;
	while (target && target->data && target->data[++i])
		ft_dprintf(fd, "%s%s", target->data[i], target->data[i + 1] ? " " : "");
}

// THIS PRINTS ALL THE CMD LINE NOT THE ARSG!!!!!
void	print_index_args(t_process* target)
{
	const bool leader = is_leader(target);
	char* index;

	index = NULL;
	ft_dprintf(STDERR_FILENO, "%s%s%s%-2s",
		leader ? "[" : " ",
		leader ? index = ft_itoa(background_index_get(g_session->nil, target)) : " ", // print "[ background index ]"
		leader ? "]" : " ",
		is_in_history(target)
		);
	print_job_args(2, target);
	free(index);
}

// Status of leader is always stopped if any elem is stopped in mode not pid
// In mode pid each elem has its Status
// must have a fds to print
// Must print exit status if != 0
// MUst stop printting Runnig for background
// Must print done for exit == 0
// Must not print the index and the history when is the result of a fork and its not stopped signal
// Must be called after a fork, after ctrl z (1 TIME) and in all the builtins

// Need 2 Modes:
	// [index][+/-](if stopped signal group)[GROUP STATUS][CORE](cmd only args for stopped)[CMD] // STANDART
	// [index][+/-](if leader and if stopped signal group)[PID][PROCESS STATUS][ARGS] // advanced (can add pipes at the end)

void	print_signal(int fd, t_process* target, int mode)
{
	int		signal;
	int		exit_status;
	t_group*	aux;

	aux = group_get(target);
	exit_status = -1;
	if (stopped_signal(signal = check_wstatus(target, &exit_status), true))
		write(fd, "\n", 1);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[TEST PRINT SIGNALS][SIGNAL: %d][WSATUS: %d][ret: %d][flags: %d]\n", signal, target->wstatus, target->ret, target->flags);
	print_index(fd, target, mode, exit_status);
	print_history(fd, target, mode, exit_status);
	print_sp(fd, target, mode, exit_status);
	print_pid(fd, target, mode);
	print_sp(fd, target, mode, exit_status);
	print_signal_(fd, target, mode, signal);
	print_exit_st(fd, exit_status);
	print_coredump(fd, target, mode);
	if (stopped_signal(signal, false) || signal == 33 || signal == 31 || target->flags & SIGNALED || exit_status > 0)
	{
		padding_spaces(fd, ft_strlen((!mode && group_condition(aux, is_active) ? "Stopped" : get_signal(signal))));
		mode ? print_job_args(fd, target) : print_group_line(fd, aux);
	}
	write(fd, "\n", 1);
}


// Not normed version
/*
void	print_signal(int fd, t_process* target, int mode)
{
	char*	freed[3];
	int		signal;
	const bool is__leader = is_leader(target);
	int		exit_status;
	t_group*	aux;

	exit_status = -1;
	aux = get_group(target);
	ft_bzero(freed, sizeof(freed));
	if (stopped_signal(signal = check_wstatus(target, &exit_status), true))
		write(fd, "\n", 1);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[TEST PRINT SIGNALS][SIGNAL: %d][WSATUS: %d][ret: %d][flags: %d]\n", signal, target->wstatus, target->ret, target->flags);
	ft_dprintf(fd, "%s%s%s%s%s%s%s%s%s%s %s",
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? "[" : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? freed[0] = ft_itoa(get_background_index(g_session->nil, target)) : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? "]" : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? is_in_history(target) : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? " " : "",
		mode ? freed[1] = ft_itoa(target->pid) : "",
		(stopped_signal_group(aux, true) || exit_status >= 0) && (!mode || (mode && is__leader)) ? " " : "",
		(!mode && stopped_signal_group(aux, false) ? "Stopped" : get_signal(signal)),
		exit_status > 0 ? " " : "",
		exit_status > 0 ? freed[2] = ft_itoa(exit_status) : "",
		(mode && __WCOREDUMP(target->wstatus)) || (!mode && group_coredump(aux)) ? "(core dumped)" : ""
	);
	if (stopped_signal(signal, false) || signal == 33 || signal == 31 || target->flags & SIGNALED || exit_status > 0)
	{
		padding_spaces(fd, ft_strlen((!mode && is_active_group(aux) ? "Stopped" : get_signal(signal))));
		mode ? print_job_args(fd, target) : print_group_line(fd, aux);
	}
	write(fd, "\n", 1);
	free(freed[0]);
	free(freed[1]);
	free(freed[2]);
}


*/
