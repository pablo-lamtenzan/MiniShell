/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:45:15 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 05:24:11 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>

// to do buffer overflow
// do returns
// check more signals like time out

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




#define SIGNALS_NB	65

t_signal_print	get_signal_print(size_t index)
{
	static const t_signal_print	signals_print[SIGNALS_NB] = {
		{.printed=true, .message="Hangup"},
		{}, // if clems handls the \n it
		{.printed=true, .message="Quit", .message_aux="(core dumped)"},
		{.printed=true, .message="Illegal instruction", .message_aux="(core dumped)"},
		{.printed=true, .message="Trace/breakpoint trap", .message_aux="(core dumped)"},
		{.printed=true, .message="Aborted", .message_aux="(core dumped)"},
		{.printed=true, .message="Bus error", .message_aux="(core dumped)"},
		{.printed=true, .message="Floating point exception", .message_aux="(core dumped)"},
		{.printed=true, .message="Killed"},
		{.printed=true, .message="User defined signal 1"},
		{.printed=true, .message="Segmentation fault", .message_aux="(core dumped)"},
		{.printed=true, .message="User defined signal 2"},
		{},
		{.printed=true, .message="Alarm clock"},
		{.printed=true, .message="Terminated"},
		{.printed=true, .message="Stack fault"},
		{},
		{},
		{.printed=true, .process_nb=1, .plus="+", .message="Stopped", .has_args=true},
		{.printed=true, .endline="\n", .process_nb=1, .plus="+", .message="Stopped", .has_args=true},
		{.printed=true, .endline="\n", .process_nb=1, .plus="+", .message="Stopped", .has_args=true},
		{.printed=true, .endline="\n", .process_nb=1, .plus="+", .message="Stopped", .has_args=true},
		{},
		{.printed=true, .message="CPU time limit exceeded", .message_aux="(core dumped)"},
		{.printed=true, .message="File size limit exceeded", .message_aux="(core dumped)"},
		{.printed=true, .message="Virtual timer expired"},
		{.printed=true, .message="Profiling timer expired"},
		{},
		{.printed=true, .message="I/O possible"},
		{.printed=true, .message="Power failure"},
		{.printed=true, .message="Bad system call", .message_aux="(core dumped)"},
		{}, // index 32
		{}, // index 33
		{.printed=true, .message="Real-time signal 0"},
		{.printed=true, .message="Real-time signal 1"},
		{.printed=true, .message="Real-time signal 2"},
		{.printed=true, .message="Real-time signal 3"},
		{.printed=true, .message="Real-time signal 4"},
		{.printed=true, .message="Real-time signal 5"},
		{.printed=true, .message="Real-time signal 6"},
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
		{.printed=true, .message="Real-time signal 26"},
		{.printed=true, .message="Real-time signal 27"},
		{.printed=true, .message="Real-time signal 28"},
		{.printed=true, .message="Real-time signal 29"},
		{.printed=true, .message="Real-time signal 30"},
	};
	return (signals_print[index - 1]);
}

// [end line] [process_nb] [token '+'] [message] [aux_msg] [args]

void	print_signals(int wsignal_index, const char** args)
{
	int	i;
	const t_signal_print signal = get_signal_print(wsignal_index);

	i = -1;
	if (!signal.printed)
		return ;
	ft_dprintf(STDERR_FILENO, "%s%s%s%s%s%s%s%s%s", signal.endline ? signal.endline : "", signal.process_nb ? "[" : "", \
			signal.process_nb ? "1" : "", signal.process_nb ? "]" : "", signal.plus ? "+  " : "", \
			signal.message , signal.message_aux ? " " : "", \
			signal.message_aux ? signal.message_aux : "", signal.has_args ? "                 " : "");
	if (signal.has_args)
		while (args && args[++i])
			ft_dprintf(STDERR_FILENO, "%s%s", args[i], args[i + 1] ? " " : "");
	write(STDERR_FILENO, "\n", 1);
}
