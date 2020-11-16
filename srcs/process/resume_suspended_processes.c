/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resume_suspended_processes.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:29:48 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 13:25:20 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signal.h>

// old
size_t			suspended_process_nb(t_process* suspended)
{
	size_t		nb;

	nb = 1;
	while (suspended && (suspended++))
		suspended = suspended->next;
	return (nb - 1);
}

// old
bool			is_suspended(int wstatus)
{
	return (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)) \
			&& (wstatus == SIGSTOP || wstatus == SIGTSTP \
			|| wstatus == SIGTTIN || wstatus == SIGTTOU));
}


// old
void            remove_suspended_process(t_process** suspended)
{
    t_process*  cp;
	t_process*	last;

    if (*suspended)
        return ;
    cp = *suspended;
	last = (*suspended)->prev;
    *suspended = (*suspended)->next;
	if (last)
		last->next = *suspended;
    free(cp);
	cp = NULL;
}

/* void                resume_suspended_processes(t_process** suspended)
{
    while (*suspended)
    {
		ft_dprintf(2, "[TEST RESUME SUSPENDED PROCESS][RESUME pid: [%d]]\n", (*suspended)->pid);
		// resume curr process
		kill((*suspended)->pid, SIGCONT);
		// wait for it: if isn t suspended remove, else repeat
		while (waitpid((*suspended)->pid, &(*suspended)->wstatus, 0) <= 0)
			;
		if (!is_suspended((*suspended)->wstatus))
		{
			remove_suspended_process(suspended);
			*suspended = (*suspended)->next;
		}
    }
} */
