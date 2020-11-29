/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   async.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:18:02 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 11:57:05 by pablo            ###   ########.fr       */
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

static void		quit_process(int signal)
{
	(void)signal;
}

static void		keyboard_interrupt(int signal)
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

void			signal_handler(void)
{
	signal(SIGTSTP, suspend_process);
	signal(SIGCHLD, zombies_catcher);
	signal(SIGQUIT, quit_process);
	signal(SIGINT, keyboard_interrupt);
	signal(SIGTERM, terminate_minishell);
}