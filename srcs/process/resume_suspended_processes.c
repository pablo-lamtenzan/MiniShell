/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resume_suspended_processes.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:29:48 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 11:56:08 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signal.h>

void            remove_suspended_process(t_process** suspended)
{
    t_process*  cp;

    if (*suspended)
        return ;
    cp = *suspended;
    *suspended = (t_process*)(*suspended)->data;
    free(cp);
}

void                resume_suspended_processes(t_process** suspended)
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
			*suspended = (t_process*)(*suspended)->data;
		}
    }
}