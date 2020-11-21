/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:26:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/20 21:10:57 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <process.h>
#include <libft.h>
#include <stdlib.h>

/*
** Allocates and int a new session
*/

t_session*		session_start()
{
	t_session*	session;

	if (!(session = ft_calloc(1, sizeof(t_session)))
			|| !(session->nil = ft_calloc(1, sizeof(t_group))))
		return (NULL);
	session->nil->next = session->nil;
	session->nil->prev = session->nil;
	session->groups = session->nil;
	ft_dprintf(2, "[NEW SESSION][\'%p\'][NIL=\'%p\']\n", session, session->nil);
	return (session);
}

void		session_end(t_session* session)
{
	t_process*	fill;
	// TO DO: kill all stopped
	
	while (!session_empty(session))
		group_pop_front(&session);
	while (session->history)
	{
		fill = session->history;
		session->history = session->history->next;
		free(fill); // delete process
	}
	free(session->nil);
	free(session);
}