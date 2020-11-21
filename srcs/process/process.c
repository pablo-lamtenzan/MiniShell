/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 00:55:34 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <execution.h>
#include <signals.h>
#include <errors.h>
/*
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
*/
// used
static int	handle_wstatus(t_process* target, t_group* curr)
{
	if (WIFEXITED(target->wstatus))
		return (WEXITSTATUS(target->wstatus));
	if (WIFSIGNALED(target->wstatus) && (target->wstatus = WTERMSIG(target->wstatus)))
		print_signals(target, curr); // change this
	return (target->wstatus + SIGNAL_BASE);
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
/*
t_exec_status	wait_processes(t_term* term, t_exec_status st)
{
    int			i;
	//t_process	*suspended;
	//t_group*	group;

    i = 0;
	//if (!(group = new_group()))
	//	return (RDR_BAD_ALLOC);
	//group_push_front(term->session, group);
    while (++i <= term->session->processes[MANAGE].pid)
    {
		if (!term->session->processes[i].pid)
			continue ;
		t_process*	fixxxx = &term->session->processes[i];
        if (!update_background(term->session, &fixxxx))
			return (RDR_BAD_ALLOC);
		term->session->processes[i] = *fixxxx; // random bullshit test
        term->st = handle_wstatus(&term->session->processes[i], term->session->nil);		
    }
	// TO DO: if group is not active
	if (term->session->groups == term->session->nil || !is_active_group(term->session->groups))
		group_pop_front(term->session);
	//if (term->session->groups->active_processes == term->session->groups->nil)
	//	group_pop_front(term->session);
	ft_bzero(term->session->processes + 1, sizeof(t_process) * term->session->processes[MANAGE].pid);
	term->session->processes[MANAGE].pid = 1;
	return (st);
}
*/

t_exec_status	wait_processes_v2(t_term* term, t_exec_status st)
{
	t_group*	group;
	t_process*	remember;

	ft_dprintf(2, "SESSION GROUPS ACTIVE PROCESSES: %p\n", term->session->groups->active_processes);
	ft_dprintf(2, "SESSION GROUPS ACTIVE PROCESSES NEXT: %p\n", term->session->groups->next ?  term->session->groups->next->active_processes : 0);
	ft_dprintf(2, "SESSION GROUP NIL: %p\n", term->session->groups->nil);
	ft_dprintf(2, "GROUP ADDR: %p\n", term->session->groups);

	
	// Catch builtins
	if (!(group = term->session->groups) || term->session->groups->active_processes == term->session->groups->nil)
	{
		group_pop_front(&term->session);
		return (st);
	}
	// For each process in the group
	remember = group->active_processes;
	while (group->active_processes != group->nil)
	{
		// put flags
		update_background(term->session, &group->active_processes);
		// get return value + print signals if there are
		term->st = handle_wstatus(group->active_processes, term->session->groups);
		ft_dprintf(2, "[WAIT PROCESSES V2][PID=\'%d\'][FLAGS=\'%d\']\n", group->active_processes->pid, group->active_processes->flags);
		group->active_processes = group->active_processes->next;
		//ft_dprintf(2, "NEXT: %p\n", group->active_processes);
	}
	group->active_processes = remember;
	// check if group is empty or not active and rm it if true
	//ft_dprintf(2, "WAIT PROCESSES after: %p\n", term->session->groups->active_processes);
	//ft_dprintf(2, "WAIT PROCESSES after->next: %p\n", term->session->groups->active_processes->next);
	//ft_dprintf(2, "WAIT PROCESSES after nil->next: %p\n", term->session->groups->nil->next);
	//ft_dprintf(2, "WAIT PROCESSES after nil->prev: %p\n", term->session->groups->nil->prev);
	//ft_dprintf(2, "NIL = %p\n", term->session->groups->nil);
	ft_dprintf(2, "[WAIT PROCESSES v2][ACT PROCESSES AT THE END: %p]\n", term->session->groups->active_processes);
	if (!is_active_group(term->session->groups)){ ft_dprintf(2, "RM GROUP IN WAITv2\n");
		group_pop_front(&term->session); }
	return (st);
}
