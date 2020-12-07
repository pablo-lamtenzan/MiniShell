/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:11:08 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:36:05 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

pid_t			process_get_leader_pid(t_group *nil, t_process *target)
{
	t_group		*groups;

	groups = nil->next;
	while (groups && groups != nil)
	{
		if (background_find(target, "PID", groups))
			return (groups->nil->next->pid);
		groups = groups->next;
	}
	return (0);
}

bool			is_leader(t_process *target)
{
	t_group		*groups;

	if (!target)
		return (false);
	groups = g_session.groups;
	while (groups != g_session.nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->pid == target->pid)
			return (true);
		groups = groups->next;
	}
	return (false);
}
