/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 08:18:13 by pablo             #+#    #+#             */
/*   Updated: 2020/11/19 17:56:59 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <process.h>
# include <stdlib.h>
# include <libft.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

void			add_process(t_process* target, t_process* prev, t_process* next)
{
	prev->next = target;
	target->prev = prev;
	next->prev = target;
	target->next = next;
}

void			remove_process(t_process** target)
{
	(*target)->next->prev = (*target)->prev;
	(*target)->prev->next = (*target)->next;
	//ft_dprintf(2, "[REMOVE PROCESS][%p]\n", *target);
	delete_process(target);
}

void			process_push_front(t_process* process, t_group** group)
{
	t_process*	fill;

	//ft_dprintf(2, "GROUP NIL BEFORE: %p\n", (*group)->nil);
	//ft_dprintf(2, "GROUP ACTIVE PROCESSES BEFORE: %p\n", (*group)->active_processes);
	fill = (*group)->active_processes;
	(*group)->nil->next = process;
	process->prev = (*group)->nil;
	process->next = fill;
	fill->prev = process;
	(*group)->active_processes = process;
	//ft_dprintf(2, "PROCESS PUSH FRONT groups->active_processes = %p\n", (*group)->active_processes);
	//ft_dprintf(2, "GROUP NIL AFTER: %p\n", (*group)->nil);
	//ft_dprintf(2, "GROUP ACTIVE PROCESSES AFTER: %p\n", (*group)->active_processes);
	//ft_dprintf(2, "GROUP NIL NEXT IS NOW: %p\n", (*group)->nil->next);
}

t_process	*new_process(pid_t pid, int wstatus, char*const* data)
{
    t_process	*process;

    if (!(process = ft_calloc(1, sizeof(t_process))))
        return (NULL);
    *process = (t_process){.pid=pid, .wstatus=wstatus, .data=data};
    return (process);
}

void			add_group(t_group* target, t_group* prev, t_group* next)
{
	prev->next = target;
	target->prev = prev;
	next->prev = target;
	target->next = next;
}

void            group_push_front(t_session* session, t_group* group)
{
	t_group*	fill;

	//ft_dprintf(2, "SESSION GROUPS IS WAS: %p\n", session->groups);
	//ft_dprintf(2, "[PUSH FRONT][NIL AT THE BEGINING: %p]\n", session->nil);
	fill = session->groups;
	session->nil->next = group;
	group->prev = session->nil;
	group->next = fill;
	fill->prev = group;
	session->groups = group;
	//ft_dprintf(2, "GROUP PUSH FRONT: session->groups = %p\n", session->groups);
	//ft_dprintf(2, "GROUP PUSH FRONT: session->groups->next = %p\n", session->groups->next);
	//ft_dprintf(2, "[PUSH FRONT][GROUP: %p]\n", group);
	//ft_dprintf(2, "SESSION GROUPS IS RIGHT NOW: %p\n", session->groups);
	//ft_dprintf(2, "SESSION GROUPS->PREV IS RIGHT NOW: %p\n", session->groups->prev);
	//ft_dprintf(2, "SESSION GROUPS NIL IS RIGHT NOW: %p\n", session->groups->prev);
}

t_group			*new_group()
{
    t_group	*group;

    if (!(group = ft_calloc(1, sizeof(t_group))))
        return (NULL);
    if (!(group->nil = ft_calloc(1, sizeof(t_process))))
        return (NULL);
	else
		group->active_processes = group->nil;
	//ft_dprintf(2, "[NEW GROUP][ALLOCATE GROUP: %p]\n", group);
	//ft_dprintf(2, "[NEW GROUP][ALLOCATE PROCESSES (=nil): %p]\n", group->active_processes);
    return (group);
}

t_session		*start_session()
{
    t_session	*session;

    if (!(session = ft_calloc(1, sizeof(t_session))))
        return (NULL);
    *session = (t_session){ .groups=NULL, .history=NULL, .processes[MANAGE].pid=1};
	if (!(session->nil = ft_calloc(1, sizeof(t_group))))
		return (NULL);
	else	
		session->groups = session->nil;
    return (session);
}

void            delete_process(t_process **target)
{
    int         i;

    i = -1;
    //while ((*target)->data && (*target)->data[++i])
   //     free((*target)->data[i]);
    //free((char**)(*target)->data);
	//(*target)->data = NULL;
	//ft_dprintf(2, "%p FREED IN DELETE PROCESS (form remove process)\n", target);
    free(*target);
	*target = NULL;
}

void			group_pop_front(t_session* session)
{
	t_group*	fill;

	fill = session->groups->next;
	//ft_dprintf(2, "[GROUP POP FRONT: %p]\n", session->groups);
	delete_group(&session->groups);
	fill->prev = session->nil;
	session->nil->next = fill;
	session->groups = fill;
	//ft_dprintf(2, "[SESSION GROUP: %p]\n", session->groups);
}

void            delete_group(t_group **target)
{
    while ((*target)->active_processes != (*target)->nil)
    {
        delete_process(&(*target)->active_processes);
        (*target)->active_processes = (*target)->active_processes->next;
    }
    free((*target)->nil);
	(*target)->nil = NULL;
	//ft_dprintf(2, "[DELETE GROUP FREE TARGET: %p]\n", *target);
	(*target) = NULL;
}

void            end_session(t_session *session)
{
	force_exit_background(session);
    while (session->groups != session->nil)
    {
        delete_group(&session->groups);
        session->groups = session->groups->next;
    }
    while (session->history)
    {
        delete_process(&session->history);
        session->history = session->history->next;
    }
	free(session->nil);
    free(session);
}

bool            update_session_history(t_session *session, t_process *update)
{
    t_process*  cp_update;
	t_process*	fill;

    if (!(cp_update = new_process(update->pid, update->wstatus, update->data)))
		return (false);
	fill = session->history;
	session->history = cp_update;
	cp_update->next = fill;
	if (fill)
		fill->prev = cp_update;
	return (true);
}

static bool		not_in_background(t_process *process, t_group *group)
{
	return (!background_find(process, "PID", group));
}

bool			update_background(t_session *session, t_process **process)
{
	t_process	*cp;
	t_process	*to_use;
	bool		allocated;
	bool		exited;

	cp = NULL;
	allocated = not_in_background(*process, session->groups);
	if (allocated && !(cp = new_process((*process)->pid, (*process)->wstatus, (*process)->data)))
		return (false);
	to_use = allocated ? cp : *process;
	ft_dprintf(2, "[UPDATE BACKGROUND][to_use addr: %p]\n", to_use);
	ft_dprintf(2, "[WAIING...][pid=\'%d\']\n", to_use->pid);
	while (waitpid(to_use->pid, &to_use->wstatus, WUNTRACED) <= 0) // WCONTINUED for test
		;
	exited = WIFEXITED(to_use->wstatus) || !WIFSTOPPED(to_use->wstatus);
	if (!allocated && exited)
		remove_process(process);
	else if (!exited)
	{
		ft_dprintf(2, "Children [%d] doesn't exited\n", to_use->pid);
		update_session_history(session, to_use);
		if (allocated)
			process_push_front(to_use, &session->groups);
	}
	else
		ft_dprintf(2, "Children [%d] exited\n", to_use->pid);
	// just for set the right return value
	if (allocated)
		(*process)->wstatus = cp->wstatus;
	return (true);
}

t_process**		background_find(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;

	i = 0;
	while (group->active_processes != group->nil)
	{
		while (i < 2 && ft_strncmp(modes[i], search_type, 3))
			i++;
		if (!i && target->pid == group->active_processes->pid)
			return (&group->active_processes);
		else if (i && target->wstatus == group->active_processes->wstatus)
			return (&group->active_processes);
		group->active_processes = group->active_processes->next;
	}
	return (NULL);
}

size_t			background_size(t_group* nil)
{
	t_process*	fill;
	t_group*	groups;
	size_t		size;

	groups = nil->next;
	fill = groups->active_processes;
	size = 1;
	while (groups != nil)
	{
		while (fill && groups->nil && (size++))
			fill = fill->next;
		groups = groups->next;
	}
	return (size - 1);
}

void			force_exit_background(t_session* session)
{
	t_process**	fill;

	while (session->groups != session->nil)
	{
		while (session->groups->active_processes != session->groups->nil)
		{
			// TO DO: for SIGTTIN SIGTTOU -> SIGHUB
			kill(session->groups->active_processes->pid, SIGCONT);
			// wait for the process to exit and then iterate
			while (waitpid(session->groups->active_processes->pid, &session->groups->active_processes->wstatus, 0) <= 0)
				;
			if (WIFEXITED(session->groups->active_processes->wstatus))
			{
				fill = &session->groups->active_processes;
				session->groups->active_processes = session->groups->active_processes->next;
				remove_process(fill);
			}
			session->groups->active_processes = session->groups->active_processes->next;
		}
		session->groups = session->groups->next;
	}
}

// have to inverse the groups
// have to create a gnil in session for manage the groups
size_t			get_background_index(t_group* nil, t_process* target)
{
	size_t		index;
	t_process*	fill;
	t_group*	groups;

	index = 0;
	groups = nil->prev;
	while (groups != nil && (index++))
	{
		fill = groups->nil->prev;
		while (fill != groups->nil)
		{
			if (fill == target)
				return (index);
			fill = fill->prev;
		}
		groups = groups->prev;
	}
	return (index);
}

pid_t			get_process_leader_pid(t_group* nil, t_process* target)
{
	t_group*	groups;

	groups = nil->next;
	while (groups && groups != nil)
	{
		if (background_find(target, "PID", groups))
			return (groups->nil->next->pid);
		groups = groups->next;
	}
	return (0);
}