/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:07:02 by pablo            ###   ########.fr       */
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
static int	handle_wstatus(t_process* target)
{
	if (PRINT_DEBUG)
		ft_dprintf(2, "[HANDLE WSTATUS: wstatus: %d]\n", target->wstatus);
	if (WIFEXITED(target->wstatus))
	{
		if (PRINT_DEBUG)
		ft_dprintf(2, "[HANDLE WSTATUS][PROCESS: \'%d\' EXITS]\n", target->pid);
		return (WEXITSTATUS(target->wstatus));
	}
	//else if (WIFSIGNALED(target->wstatus))
		//target->wstatus = WTERMSIG(target->wstatus);
	//else if (WIFSTOPPED(target->wstatus))
		//target->wstatus = WSTOPSIG(target->wstatus);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[HANDLE WSTATUS][PROCESS; \'%d\' IS SIGNALED OR STOPPED]\n", target->pid);
	print_signal(STDERR_FILENO, target, STANDART);
	return (SIGNAL_BASE + (WIFSTOPPED(target->wstatus) ? WSTOPSIG(target->wstatus) : WTERMSIG(target->wstatus)));
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
        g_session.st = handle_wstatus(&term->session->processes[i], term->session->nil);		
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
	(void)term;

	if (PRINT_DEBUG){
	ft_dprintf(2, "SESSION GROUPS ACTIVE PROCESSES: %p\n", g_session.groups->active_processes);
	ft_dprintf(2, "SESSION GROUPS ACTIVE PROCESSES NEXT: %p\n", g_session.groups->next ?  g_session.groups->next->active_processes : 0);
	ft_dprintf(2, "SESSION GROUP NIL: %p\n", g_session.groups->nil);
	ft_dprintf(2, "GROUP ADDR: %p\n", g_session.groups);}

	/*
	if (g_session.groups->next && g_session.groups->next != g_session.nil)
	{

		ft_dprintf(2, "-----------> TEST: %d\n", g_session.groups->next->active_processes->flags);

		if (g_session.groups->next->active_processes->flags & SIGNALED && !(g_session.groups->next->active_processes->flags & KILLED))
		g_session.groups->active_processes->flags &= ~SIGNALED;
		if (g_session.groups->next->active_processes->flags & KILLED)
		{
			g_session.groups->next->active_processes->flags &= ~KILLED;
			ft_dprintf(2, "[REMOVE KILL FLAG]\n");
		}
	}
	*/
	
	// Catch builtins
	// TO DO: Check if a need here to calc the return
	if (!(group = g_session.groups) || g_session.groups->active_processes == g_session.groups->nil)
	{
		if (PRINT_DEBUG){
		if (g_session.groups->next && g_session.groups->next->active_processes)
			ft_dprintf(2, "[WAIT V2][PREVIOUS GROUP LEADER FLAGS ARE: %d][\'%p\']\n", g_session.groups->next->active_processes->flags, g_session.groups->next->active_processes);}
		if (!(g_session.groups->active_processes->flags & SIGNALED))
			group_pop_front();
		if (PRINT_DEBUG) {
		if (g_session.groups && g_session.groups->active_processes)
			ft_dprintf(2, "[WAIT V2][PREVIOUS GROUP LEADER FLAGS ARE: %d][\'%p\']\n", g_session.groups->active_processes->flags, g_session.groups->active_processes);}
		print_endzombies();
		remove_exited_zombies();
		return (st);
	}
	// For each process in the group
	remember = group->active_processes;
	while (group->active_processes != group->nil)
	{
		// put flags
		g_session.open_print = false;
		update_background(&group->active_processes, true);
		// get return value + print signals if there are
		g_session.st = handle_wstatus(group->active_processes);
		if (PRINT_DEBUG)
			ft_dprintf(2, "[WAIT PROCESSES V2][PID=\'%d\'][FLAGS=\'%d\']\n", group->active_processes->pid, group->active_processes->flags);
		group->active_processes = group->active_processes->next;
		//ft_dprintf(2, "NEXT: %p\n", group->active_processes);
	}
	group->active_processes = remember;
	// check if group is empty or not active and rm it if true
	//ft_dprintf(2, "WAIT PROCESSES after: %p\n", g_session.groups->active_processes);
	//ft_dprintf(2, "WAIT PROCESSES after->next: %p\n", g_session.groups->active_processes->next);
	//ft_dprintf(2, "WAIT PROCESSES after nil->next: %p\n", g_session.groups->nil->next);
	//ft_dprintf(2, "WAIT PROCESSES after nil->prev: %p\n", g_session.groups->nil->prev);
	//ft_dprintf(2, "NIL = %p\n", g_session.groups->nil);

	//ft_dprintf(2, "[GROUP RETURN STATUS BEFORE IS: %d]\n", g_session.st);
	get_group_return();
	print_endzombies();
	
	if (PRINT_DEBUG)
		ft_dprintf(2, "[WAIT PROCESSES v2][ACT PROCESSES AT THE END: %p]\n", g_session.groups->active_processes);
	if (!is_active_group(g_session.groups))
		group_pop_front();
	zombies_list_purge_exited_groups();
	ft_dprintf(2, "[GROUP RETURN STATUS IS: %d]\n", g_session.st);
	return (st);
}
