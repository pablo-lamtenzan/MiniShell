/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 13:26:48 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <execution.h>
#include <signals.h>
#include <errors.h>

static int	handle_wstatus(int wstatus, char*const* av, t_process* suspended)
{
	int		i;

	i = -1;
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
		print_signals(wstatus, (const char**)av, suspended);
	return (wstatus + SIGNAL_BASE);
}

static void		process_push_back(t_process** root, t_process* n)
{
	t_process*	tmp;

	tmp = *root;
	while (tmp->data)
		tmp = (t_process*)tmp->data;
	tmp->data = n;
}

static void		process_push_front(t_process** root, t_process* n)
{
	t_process*	tmp;

	tmp = *root;
	*root = n;
	(*root)->data =  tmp;
}

// use for print signals
size_t			suspended_process_nb(t_process* suspended)
{
	t_process*	tmp;
	size_t		nb;

	tmp = suspended;
	nb = 1;
	while (tmp && (nb++))
		tmp = (t_process*)tmp->data;
	return (nb - 1);
}

bool			is_suspended(int wstatus)
{
	return (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)) \
			&& (wstatus == SIGSTOP || wstatus == SIGTSTP \
			|| wstatus == SIGTTIN || wstatus == SIGTTOU));
}

t_exec_status	wait_processes(t_term* term, t_exec_status st)
{
    int			i;
	t_process	*suspended;

    i = 0;
    while (++i < term->processes[MANAGE].pid)
    {
        while (waitpid(term->processes[i].pid, &term->processes[i].wstatus, 0) <= 0)
            ;
		if (is_suspended(term->processes[i].wstatus))
		{
			if (!(suspended = malloc(sizeof(t_process))))
				return (BAD_ALLOC);
			*suspended = term->processes[i];
			suspended->data = NULL;
			if (term->suspended_processes)
				process_push_front(&term->suspended_processes, suspended);
		}
        term->st = handle_wstatus(&term->processes[i].wstatus, (char*const*)term->processes[i].data, term->suspended_processes);
    }
	ft_bzero(term->processes, sizeof(term->processes));
	return (st);
}