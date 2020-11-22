/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:45:15 by pablo             #+#    #+#             */
/*   Updated: 2020/11/22 06:30:13 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>
#include <sys/wait.h>
#include <process.h>

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




static t_signal_print	get_60_to_80(size_t index)
{
	static const t_signal_print signals_print[5] = {
		{.printed=true, .message="Real-time signal 26"},
		{.printed=true, .message="Real-time signal 27"},
		{.printed=true, .message="Real-time signal 28"},
		{.printed=true, .message="Real-time signal 29"},
		{.printed=true, .message="Real-time signal 30"}
	};
	return (signals_print[index - 1]);
}

static t_signal_print	get_40_to_60(size_t index)
{
	static const t_signal_print signals_print[20] = {
		{.printed=true, .message="Real-time signal 7"},
		{.printed=true, .message="Real-time signal 8"},
		{.printed=true, .message="Real-time signal 9"},
		{.printed=true, .message="Real-time signal 10"},
		{.printed=true, .message="Real-time signal 11"},
		{.printed=true, .message="Real-time signal 12"},
		{.printed=true, .message="Real-time signal 13"},
		{.printed=true, .message="Real-time signal 14"},
		{.printed=true, .message="Real-time signal 15"},
		{.printed=true, .message="Real-time signal 16"},
		{.printed=true, .message="Real-time signal 17"},
		{.printed=true, .message="Real-time signal 18"},
		{.printed=true, .message="Real-time signal 19"},
		{.printed=true, .message="Real-time signal 20"},
		{.printed=true, .message="Real-time signal 21"},
		{.printed=true, .message="Real-time signal 22"},
		{.printed=true, .message="Real-time signal 23"},
		{.printed=true, .message="Real-time signal 24"},
		{.printed=true, .message="Real-time signal 25"},
	};
	return (index <= 60 ? signals_print[index - 1] : get_60_to_80(index - 60));
}

static t_signal_print	get_20_to_40(size_t index)
{
	static const t_signal_print	signals_print[20] = {
		{.printed=true, .endline="\n", .process_nb=1, .plus=true, \
				.message="Stopped", .has_args=true},
		{.printed=true, .endline="\n", .process_nb=1, .plus=true, \
				.message="Stopped", .has_args=true}, {},
		{.printed=true, .message="CPU time limit exceeded", .message_aux=CORE},
		{.printed=true, .message="File size limit exceeded", .message_aux=CORE},
		{.printed=true, .message="Virtual timer expired"},
		{.printed=true, .message="Profiling timer expired"}, {},
		{.printed=true, .message="I/O possible"},
		{.printed=true, .message="Power failure"},
		{.printed=true, .message="Bad system call", .message_aux=CORE}, {}, {},
		{.printed=true, .message="Real-time signal 0"},
		{.printed=true, .message="Real-time signal 1"},
		{.printed=true, .message="Real-time signal 2"},
		{.printed=true, .message="Real-time signal 3"},
		{.printed=true, .message="Real-time signal 4"},
		{.printed=true, .message="Real-time signal 5"},
		{.printed=true, .message="Real-time signal 6"}
	};
	return (index <= 40 ? signals_print[index - 1] : get_40_to_60(index - 40));
}

t_signal_print	get_signal_print(size_t index)
{
	static const t_signal_print	signals_print[20] = {
		{.printed=true, .message="Hangup"},
		{}, // if clems handls the \n it
		{.printed=true, .message="Quit", .message_aux=CORE},
		{.printed=true, .message="Illegal instruction", .message_aux=CORE},
		{.printed=true, .message="Trace/breakpoint trap", .message_aux=CORE},
		{.printed=true, .message="Aborted", .message_aux=CORE},
		{.printed=true, .message="Bus error", .message_aux=CORE},
		{.printed=true, .message="Floating point exception", .message_aux=CORE},
		{.printed=true, .message="Killed"},
		{.printed=true, .message="User defined signal 1"},
		{.printed=true, .message="Segmentation fault", .message_aux=CORE},
		{.printed=true, .message="User defined signal 2"}, {},
		{.printed=true, .message="Alarm clock"},
		{.printed=true, .message="Terminated"},
		{.printed=true, .message="Stack fault"}, {}, {},
		{.printed=true, .process_nb=1, .plus=true, .message="Stopped", \
				.has_args=true},
		{.printed=true, .endline="\n", .process_nb=1, .plus=true, \
				.message="Stopped", .has_args=true}
	};
	return (index <= 20 ? signals_print[index - 1] : get_20_to_40(index - 20));
}

// [end line] [process_nb] [token '+'] [message] [aux_msg] [args]

// token + to prev pid, token - to prev - 1 pid
// need to remember to its pid and cmp



void	print_signals(t_process* target, t_group* nil)
{
	int						i;
	const t_signal_print	signal = get_signal_print(target->wstatus);
	char					*addr;

	i = -1;
	addr = NULL;
	if (!signal.printed)
		return ;
	ft_dprintf(STDERR_FILENO, "%s%s%s%s%s%s%s%s%s", \
			signal.endline ? signal.endline : "", \
			signal.process_nb ? "[" : "", \
			signal.process_nb ? (addr = ft_itoa(get_background_index(nil, target))) : "", \
			signal.process_nb ? "]" : "", \
			signal.plus ? "+  " : "", \
			signal.message , \
			signal.message_aux ? " " : "", \
			signal.message_aux ? signal.message_aux : "", \
			signal.has_args ? "                 " : ""\
			);
	if (signal.has_args)
		while (target->data && target->data[++i])
			ft_dprintf(STDERR_FILENO, "%s%s", target->data[i], target->data[i + 1] ? " " : "");
	write(STDERR_FILENO, "\n", 1);
	free(addr);
}

/* New Version */

// SIGSTOP 127


# define PRINT_PID		1
# define PRINT_JOB_ARGS	2
# define PRINT_JOBS_CMD	4

// TO DO: CORE DUMPED WORK ONLY WITH THE WSTATUS ...

void	print_signal_v2(t_session* session, t_process* target, int flags)
{
	static const char*		signals[31] = {
		"Hangup", "", "Quit", "Illegal instruction", "Trace/breakpoint trap", "Aborted", \
		"Bus error", "Floating point exception", "Killed", "User defined signal 1", \
		"Segmentation fault", "User defined signal 2","", "Alarm clock", "Terminated", "Stack fault", \
		"", "", "Stopped", "Stopped", "Stopped", "Stopped", "", "CPU time limit exceeded", \
		"File size limit exceeded", "Virtual timer expired", "Profiling timer expired", "I/O possible", \
		"Power failure", "Bad system call"};
	const int signal = target->wstatus;
	const bool leader = is_leader(session, target);
	char* pid;
	char* index;
	int		i;

	pid = NULL;
	index = NULL;
	ft_dprintf(2, "[PRINT SIGNAL][SIGNAL IS: \'%d\']\n", signal);

	// format: [[suspended/backgound index][history index][spaces]or[paddin spaces]+[pid if pid][siganls[signal]][WCOREDUMP][CONST SPACES][ARGS OR CMD]
	if (signal >= 19 && signal <= 22)
		write(2, "\n", 1);
	ft_dprintf(STDERR_FILENO, "%s%s%s%s %s %s %-16s",
		leader ? "[" : " ",
		leader ? index = ft_itoa(get_background_index(session->nil, target)) : " ", // print "[ background index ]"
		leader ? "]" : " ",
		is_in_history(session, target), // print history index
		flags & PRINT_PID ? pid = ft_itoa(target->pid) : "", // print pid
		signals[signal - 1],
		WCOREDUMP(signal) ? "(core dumped)" : ""
		);
	if (flags & PRINT_JOBS_CMD)
		ft_dprintf(STDERR_FILENO, "%s", "COMMAND LINE TRIMMED BY SEPARATORS HERE");
	else if (flags & PRINT_JOB_ARGS && target->data)
	{
		i = -1;
		while (target->data[++i])
			ft_dprintf(STDERR_FILENO, "%s%s", target->data[i], target->data[i + 1] ? " " : "");
	}
	write(2, "\n", 1);
	free(pid);
	free(index);
}
