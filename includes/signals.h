#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdlib.h>
# include <signals.h>

# include <libft.h>

# define KILL_SIG_PREFIX	"SIG"

typedef struct	s_signal
{
	const char	*name;
	int			value;
}				t_signal;

static const t_signal	g_signals[] = {
	{"HUP", SIGHUP}, {"INT", SIGINT}, {"QUIT", SIGQUIT}, {"ILL", SIGILL},
	{"TRAP", SIGTRAP}, {"ABRT", SIGABRT}, {"EMT", SIGEMT}, {"FPE", SIGFPE},
	{"KILL", SIGKILL}, {"BUS", SIGBUS}, {"SEGV", SIGSEGV}, {"SYS", SIGSYS},
	{"PIPE", SIGPIPE}, {"ALRM", SIGALRM}, {"TERM", SIGTERM}, {"URG", SIGURG},
	{"STOP", SIGSTOP}, {"TSTP", SIGTSTP}, {"CONT", SIGCONT}, {"CHLD", SIGCHLD},
	{"TTIN", SIGTTIN}, {"TTOU", SIGTTOU}, {"IO", SIGIO}, {"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ}, {"VTALRM", SIGVTALRM}, {"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, {"INFO", SIGINFO}, {"USR1", SIGUSR1},
	{"USR2", SIGUSR2},
# ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
# endif
# ifdef SIGPWR
	{"PWR", SIGPWR},
# endif
};

t_signal	*signal_get_value(t_list *signals, char *name);
t_signal	*signal_get_name(t_list *signals, int value);

#endif