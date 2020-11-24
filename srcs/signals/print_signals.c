/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:45:15 by pablo             #+#    #+#             */
/*   Updated: 2020/11/24 23:36:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>
#include <sys/wait.h>
#include <job_control.h>
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
		leader ? index = ft_itoa(get_background_index(g_session->nil, target)) : " ", // print "[ background index ]"
		leader ? "]" : " ",
		is_in_history(target)
		);
	print_job_args(2, target);
	free(index);
}

# define PRINT_PID		1
# define PRINT_JOB_ARGS	2
# define PRINT_JOBS_CMD	4

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

# define STANDART		0
# define ADVANCED		1

const char*	get_signal(int index)
{
	static const char*		signals[33] = {
		"Hangup", "", "Quit", "Illegal instruction", "Trace/breakpoint trap", "Aborted", \
		"Bus error", "Floating point exception", "Killed", "User defined signal 1", \
		"Segmentation fault", "User defined signal 2","", "Alarm clock", "Terminated", "Stack fault", \
		"", "", "Stopped", "Stopped", "Stopped", "Stopped", "", "CPU time limit exceeded", \
		"File size limit exceeded", "Virtual timer expired", "Profiling timer expired", "I/O possible", \
		"Power failure", "Bad system call", "Done", "Exit", "Running"
	};
	return (signals[index - 1]);
}

int		check_wstatus(t_process* target, int *exit_status)
{
	if (target->flags & BACKGROUND)
		return (33);
	if (WIFEXITED(target->wstatus))
	{
		if ((*exit_status = WEXITSTATUS(target->wstatus)))
			return (32);
		else
			return (31);
	}
	else if (WIFSTOPPED(target->wstatus))
		return (WSTOPSIG(target->wstatus));
	else
		return (WTERMSIG(target->wstatus));
}

bool	stopped_signal(int signal, bool ignore_tstp)
{
	return (signal == SIGSTOP || (!ignore_tstp && signal == SIGTSTP) || signal == SIGTTIN || signal == SIGTTOU);
}

bool	stopped_signal_group(t_group* group, bool wcheck)
{
	t_process	*leader;
	
	leader = group->active_processes;
	while (leader != group->nil)
	{
		if (leader->flags & STOPPED || (wcheck && WIFSTOPPED(leader->wstatus)))
			return (true);
		leader = leader->next;
	}
	return (false);
}

bool	group_coredump(t_group* group)
{
	t_process* leader;

	leader = group->active_processes;
	while (leader != group->nil)
	{
		if (__WCOREDUMP(leader->wstatus))
			return (true);
		leader = leader->next;
	}
	return (false);	
}

# define SIGNAL_PADDING 18

void	padding_spaces(int fd, size_t alreaddy_written)
{
	int i;

	i = -1;
	while (++i < SIGNAL_PADDING - (int)alreaddy_written)
		write(fd, " ", 1);
}

void	print_group_line(int fd, t_group *group)
{
	int i;
	
	i = -1;
	while (group && group->input && group->input[++i])
		ft_dprintf(fd, "%s%s", group->input[i], group->input[i + 1] ? " " : "");
}

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
	ft_dprintf(2, "[TEST PRINT SIGNALS][SIGNAL: %d]\n", signal);
	ft_dprintf(fd, "%s%s%s%s%s%s%s%s%s %s",
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? "[" : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? freed[0] = ft_itoa(get_background_index(g_session->nil, target)) : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? "]" : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? is_in_history(target) : (mode ? " " : ""),
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? " " : "",
		mode ? freed[1] = ft_itoa(target->pid) : "",
		(stopped_signal_group(aux, true) || !exit_status) && (!mode || (mode && is__leader)) ? " " : "",
		(!mode && stopped_signal_group(aux, false) ? "Stopped" : get_signal(signal)),
		exit_status > 0 ? freed[2] = ft_itoa(exit_status) : "",
		(mode && __WCOREDUMP(target->wstatus)) || (!mode && group_coredump(aux)) ? "(core dumped)" : ""
	);
	if (stopped_signal(signal, false) || signal == 33 || signal == 31)
	{
		padding_spaces(fd, ft_strlen((!mode && is_active_group(aux) ? "Stopped" : get_signal(signal))));
		mode ? print_job_args(fd, target) : print_group_line(fd, aux);
	}
	write(fd, "\n", 1);
	free(freed[0]);
	free(freed[1]);
	free(freed[2]);
}




void	print_signal_v2(t_process* target, int flags)
{
	static const char*		signals[32] = {
		"Hangup", "", "Quit", "Illegal instruction", "Trace/breakpoint trap", "Aborted", \
		"Bus error", "Floating point exception", "Killed", "User defined signal 1", \
		"Segmentation fault", "User defined signal 2","", "Alarm clock", "Terminated", "Stack fault", \
		"", "", "Stopped", "Stopped", "Stopped", "Stopped", "", "CPU time limit exceeded", \
		"File size limit exceeded", "Virtual timer expired", "Profiling timer expired", "I/O possible", \
		"Power failure", "Bad system call", "Done", "Exit"};
	int signal;
	const bool leader = is_leader(target);
	char* pid;
	char* index;
	char*	rett;
	int ret = 0;

	pid = NULL;
	index = NULL;
	rett = NULL;
	if (WIFEXITED(target->wstatus))
	{
		if (WEXITSTATUS(target->wstatus) == 0)
			signal = 31;
		else
		{
			signal = 32;
			ret = WEXITSTATUS(target->wstatus);
		}
	}
	else if (WIFSIGNALED(target->wstatus))
		signal = WTERMSIG(target->wstatus);
	else
		signal = WSTOPSIG(target->wstatus);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[PRINT SIGNAL][SIGNAL IS: \'%d\']\n", signal);

	// format: [[suspended/backgound index][history index][spaces]or[paddin spaces]+[pid if pid][siganls[signal]][WCOREDUMP][CONST SPACES][ARGS OR CMD]
	if (signal >= 19 && signal <= 22 && signal != SIGTSTP)
		write(2, "\n", 1);
	ft_dprintf(STDERR_FILENO, "%s%s%s%s %s %s %s %-16s",
		leader ? "[" : " ",
		leader ? index = ft_itoa(get_background_index(g_session->nil, target)) : " ", // print "[ background index ]"
		leader ? "]" : " ",
		is_in_history(target), // print history index
		flags & PRINT_PID ? pid = ft_itoa(target->pid) : "", // print pid
		// PROBLEM HERE: IF PROCESS IS IN BACKGROUND NEED TO REMOVE THE FLAG WHEN IT END... (in zombies catcher)
		target->flags & BACKGROUND ? "Runnnig" : signals[signal - 1],
		ret ? rett = ft_itoa(ret) : "",
		WCOREDUMP(target->wstatus) ? "(core dumped)" : ""
		);
	if (flags & PRINT_JOBS_CMD)
	{
		t_group* g = get_group(target);
		int i = -1;
		while (g && g->input && g->input[++i])
			ft_dprintf(STDERR_FILENO, "%s%s", g->input[i], g->input[i + 1] ? " " : "");
	}
	else if (flags & PRINT_JOB_ARGS && target->data)
		print_job_args(2,target);
	write(STDERR_FILENO, "\n", 1);
	free(pid);
	free(index);
	free(rett);
}
