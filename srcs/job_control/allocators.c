/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:22:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 06:32:48 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

t_session			*session_start()
{
	ft_bzero(&g_session, sizeof(t_session));
	if (!(g_session.nil = ft_calloc(1, sizeof(t_group))))
		return (NULL);
	g_session.nil->next = g_session.nil;
	g_session.nil->prev = g_session.nil;
	g_session.groups = g_session.nil;
	return (&g_session);
}

t_group				*group_new()
{
	t_group			*group;

	if (!(group = ft_calloc(1, sizeof(t_group)))
			|| !(group->nil = ft_calloc(1, sizeof(t_process))))
		return (NULL);
	group->nil->next = group->nil;
	group->nil->prev = group->nil;
	group->active_processes = group->nil;
	return (group);
}

t_process			*process_new(pid_t pid, int wstatus, char*const* data)
{
	t_process		*process;

	if (!(process = ft_calloc(1, sizeof(t_process))))
		return (NULL);
	*process = (t_process){.pid=pid, .wstatus=wstatus, .data=data, .ret=-1};
	return (process);
}

t_deadzombie		*deadzombie_new(t_process* target)
{
	t_deadzombie	*endzombie;
	
	if (!(endzombie = ft_calloc(1, sizeof(t_deadzombie))))
		return (NULL);
	*endzombie = (t_deadzombie){.deadzombie=target};
	return (endzombie);
}
