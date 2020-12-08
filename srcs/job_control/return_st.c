/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_st.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 21:12:39 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 21:17:00 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

static void		set_ret(void)
{
	if (g_session.groups->active_processes->prev == g_session.groups->nil)
		g_session.st = SIGNAL_BASE + \
				WSTOPSIG(g_session.groups->active_processes->wstatus);
	else
		g_session.st = SIGNAL_BASE + \
				g_session.groups->active_processes->prev->ret;
}

void			group_return_handler(void)
{
	t_process	*leader;

	if (group_condition(g_session.groups, is_active))
	{
		leader = g_session.groups->active_processes;
		while (g_session.groups->active_processes != g_session.groups->nil)
		{
			if (g_session.groups->active_processes->flags \
						& (STOPPED | BACKGROUND))
			{
				set_ret();
				break ;
			}
			g_session.groups->active_processes = \
				g_session.groups->active_processes->next;
		}
		g_session.groups->active_processes = leader;
	}
	else
		g_session.st = g_session.groups->nil->prev->ret;
}
