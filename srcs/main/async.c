/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:18:02 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 20:51:18 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <term/term.h>
#include <signals.h>

static void		suspend_process(int signal)
{
	(void)signal;
	write(STDERR_FILENO, "\n", 1);
	if (g_session.flags & OPEN_PRINT)
		print_signal(STDERR_FILENO, \
			g_session.groups->active_processes, STANDART);
}

static void		do_nothing(int signal)
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
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}

static void		interrupt_line(int signal)
{
	(void)signal;
	write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1);
	if (g_term.msg)
		write(STDERR_FILENO, g_term.msg, g_term.msg->len);
	if (g_term.line && g_term.line->len)
	{
		g_term.line->len = 0;
		if (g_term.line->data)
			*g_term.line->data = '\0';
	}
}

void			init_signal_handler(bool interactive)
{
	signal(SIGTSTP, &suspend_process);
	signal(SIGCHLD, &zombies_catcher);
	signal(SIGTERM, &terminate_minishell);
	signal(SIGHUP, &terminate_minishell);
	signal(SIGQUIT, &do_nothing);
	signal(SIGINT, (interactive) ? &interrupt_line : &do_nothing);
}
