/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:18:02 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 11:43:51 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <term/term.h>
#include <signals.h>

static void		suspend_process(int signal)
{
	(void)signal;
	// TO DO: Block this before free all
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
	// DO TO: Can be leaks if per example minish is allocating mem in a aux fct
	// We can do another global with the references of all the not-global allocation and then free them in this call
	(void)signal;
	term_destroy();
	session_end();
	exit(g_session.st);
}

/*
void			ignore_all_signals(void)
{
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
}
*/

void			init_signal_handler(void)
{
	signal(SIGTSTP, suspend_process);
	signal(SIGCHLD, zombies_catcher);
	signal(SIGQUIT, do_nothing);
	signal(SIGINT, do_nothing);
	signal(SIGTERM, terminate_minishell);
	signal(SIGHUP, terminate_minishell);
}