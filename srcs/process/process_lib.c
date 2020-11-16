/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 08:18:13 by pablo             #+#    #+#             */
/*   Updated: 2020/11/16 08:18:14 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <process.h>
# include <stdlib.h>
# include <libft.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

void            delete_process(t_process* target);
t_process*		background_find(t_process* target, const char* search_type, t_group* group);

void            add_process(t_process* target, t_process* prev, t_process* next)
{
    prev->next = target;
    target->prev = prev;
    next->prev = target;
    target->next = next;
}

void            remove_process(t_process* target)
{
    target->next->prev = target->prev;
    target->prev->next = target->next;
    delete_process(target);
}

void            process_push_front(t_process* process, t_group* group)
{
    add_process(process, group->nil, group->nil->next ? group->nil->next : group->nil);
}

t_process*      new_process(__pid_t pid, int wstatus, char*const* data)
{
    t_process*  process;

    if (!(process = ft_calloc(1, sizeof(t_process))))
        return (NULL);
    *process = (t_process){.pid=pid, .wstatus=wstatus, .data=data};
    return (process);
}

void            group_push_front(t_session* session, t_group* target)
{
    t_group*  fill;

    fill = session->groups;
    session->groups = target;
	target->next = fill;
}

t_group*        new_group()
{
    t_group*    group;

    if (!(group = ft_calloc(1, sizeof(t_group))))
        return (NULL);
    if (!(group->nil = ft_calloc(1, sizeof(t_process))))
        return (NULL);
    return (group);
}

t_session*      start_session()
{
    t_session*  session;

    if (!(session = ft_calloc(1, sizeof(t_session))))
        return (NULL);
    *session = (t_session){ .groups=NULL, .history=NULL, .processes[MANAGE]=1};
    return (session);
}

void            delete_process(t_process* target)
{
    int         i;

    i = -1;
    while (target->data[++i])
        free(target->data[i]);
    free(target->data);
    free(target);
}

void            delete_group(t_group* target)
{
    while (target->active_processes != target->nil)
    {
        delete_process(target->active_processes);
        target->active_processes = target->next;
    }
    free(target->nil);
}

void            end_session(t_session* session)
{
    while (session->groups)
    {
        delete_group(session->groups);
        session->groups = session->groups->next;
    }
    while (session->history)
    {
        delete_process(session->history);
        session->history = session->history->next;
    }
    free(session);
}

bool            update_session_history(t_session* session, t_process* update)
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

static bool		not_in_background(t_process* process, t_group* group)
{
	return (background_find(process, "PID", group));
}

bool			update_background(t_session* session, t_process* process)
{
	t_process*	cp;
	t_process*	to_use;
	bool		allocated;
	bool		exited;

	allocated = false;
	if ((allocated = not_in_background(process, session->groups)) \
		&& !(cp = new_process(process->pid, process->wstatus, process->data)))
			return (false);
	to_use = allocated ? cp : process;
	while (waitpid(to_use->pid, &to_use->wstatus, 0) <= 0)
		;
	exited = WIFEXITED(to_use->pid);
	if (!allocated && exited)
		delete_process(to_use);
	else if (!exited)
	{
		update_session_history(session, to_use);
		if (allocated)
			process_push_front(to_use, session->groups);
	}
	return (true);
}

t_process*		background_find(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;

	i = 0;
	while (group->active_processes != group->nil)
	{
		while (i < 2 && ft_strncmp(modes[i], search_type, 3))
			i++;
		if (!i && target->pid == group->active_processes->pid)
			return (group->active_processes);
		else if (i && target->wstatus == group->active_processes->wstatus)
			return (group->active_processes);
	}
	return (NULL);
}

size_t			background_size(t_group* group)
{
	t_process*	fill;
	size_t		size;

	fill = group->active_processes;
	size = 1;
	while (fill && group->nil && (size++))
		fill = fill->next;
	return (size - 1);
}

void			force_exit_background(t_session* session)
{
	t_process*	fill;

	while (session->groups)
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
				fill = session->groups->active_processes;
				session->groups->active_processes = session->groups->active_processes->next;
				remove_process(fill);
			}
		}
	}
}