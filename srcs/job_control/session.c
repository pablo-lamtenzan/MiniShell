/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:26:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 15:58:34 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

/*
** Allocates and int a new session
*/

t_session*		session_start()
{
	if (!(g_session = ft_calloc(1, sizeof(t_session)))
			|| !(g_session->nil = ft_calloc(1, sizeof(t_group))))
		return (NULL);
	g_session->nil->next = g_session->nil;
	g_session->nil->prev = g_session->nil;
	g_session->groups = g_session->nil;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[NEW SESSION][\'%p\'][NIL=\'%p\']\n", g_session, g_session->nil);
	return (g_session);
}

void		session_end()
{
	t_history*	fill;
	// TO DO: kill all stopped
	
	force_exit_background();
	while (!session_empty())
		group_pop_front();
	while (g_session->hist)
	{
		fill = g_session->hist;
		g_session->hist = g_session->hist->next;
		free(fill); // delete process
	}
	free(g_session->nil);
	free(g_session);
	g_session = NULL;
}