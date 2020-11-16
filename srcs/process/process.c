/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 15:25:29 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <execution.h>
#include <signals.h>
#include <errors.h>

// old
void		ft_swap(int* a, int* b)
{
	const int tmp = *a;

	*a = *b;
	*b = tmp;
}

// old
void		update_used_pids(int new, pid_t** used_pids)
{
	ft_swap(used_pids[LAST], used_pids[PENULTIMATE]);
	*used_pids[LAST] = new;
}

// used
static int	handle_wstatus(int wstatus, char*const* av, t_group* curr)
{
	int		i;

	i = -1;
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
		print_signals(wstatus, (const char**)av, curr);
	return (wstatus + SIGNAL_BASE);
}

/*
static void		process_push_back(t_process** root, t_process* n) // unused
{
	t_process*	tmp;

	tmp = *root;
	while (tmp->data)
		tmp = (t_process*)tmp->data;
	tmp->data = n;
}
*/

// old
/*
static void		process_push_front(t_process** root, t_process* n)
{
	t_process*	tmp;

	tmp = *root;
	*root = n;
	(*root)->next = tmp;
	if (tmp)
		tmp->prev = *root;
}
*/

// used
t_exec_status	wait_processes(t_term* term, t_exec_status st)
{
    int			i;
	//t_process	*suspended;
	t_group*	group;

    i = 0;
    while (++i < term->session->processes[MANAGE].pid)
    {
		if (!(group = new_group()))
			return (RDR_BAD_ALLOC);
		group_push_front(term->session, group);
        if (update_background(term->session, &term->session->processes[i]))
			return (RDR_BAD_ALLOC);
        term->st = handle_wstatus(term->session->processes[i].wstatus, (char*const*)term->session->processes[i].data, term->session->groups);
    }
	ft_bzero(term->session->processes, sizeof(t_process) * term->session->processes[MANAGE].pid);
	return (st);
}
