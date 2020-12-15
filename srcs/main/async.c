/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:18:02 by pablo             #+#    #+#             */
/*   Updated: 2020/12/13 03:17:41 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <term/term.h>
#include <signals_print.h>
#include <cross_platform_signals.h>
#include <signal_handler.h>

void			suspend_process(int signal)
{
	(void)signal;
	write(STDERR_FILENO, "\n", 1);
	if (g_session.flags & OPEN_PRINT)
		print_signal(STDERR_FILENO, \
			g_session.groups->active_processes, STANDART);
}

void			do_nothing(int signal)
{
	(void)signal;
}

static void		terminate_minishell(int signal)
{
	const int	exit_st = g_session.st;

	(void)signal;
	term_destroy();
	session_end(&g_session);
	exit(exit_st);
}

void			ignore_all_signals(void)
{
	uint64_t			i;
	static const int	cross_signals[] = {
		SIGTSTP, SIGCHLD, SIGQUIT, SIGINT, SIGTERM, SIGHUP,
		SIGILL, SIGABRT, SIGBUS, SIGFPE, SIGUSR1, SIGSEGV,
		SIGUSR2, SIGPIPE, SIGALRM, SIGCONT, SIGTTIN, SIGTTOU,
		SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
	};

	i = 0;
	while (i < sizeof(cross_signals) / sizeof(*cross_signals))
		signal(cross_signals[i++], SIG_IGN);
}

void			init_signal_handler(void)
{
	signal(SIGTSTP, &suspend_process);
	signal(SIGCHLD, &zombies_catcher);
	signal(SIGTERM, &terminate_minishell);
	signal(SIGHUP, &terminate_minishell);
	signal(SIGQUIT, &do_nothing);
	signal(SIGINT, &do_nothing);
}
