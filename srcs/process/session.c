/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:26:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 07:50:12 by pablo            ###   ########.fr       */
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
	if (PRINT_DEBUG)
		ft_dprintf(2, "[NEW SESSION][\'%p\'][NIL=\'%p\']\n", session, session->nil);
	return (session);
}

void		session_end(t_session* session)
{
	t_history*	fill;
	// TO DO: kill all stopped
	
	force_exit_background(session);
	while (!session_empty(session))
		group_pop_front(&session);
	while (session->hist)
	{
		fill = session->hist;
		session->hist = session->hist->next;
		free(fill); // delete process
	}
	free(session->nil);
	free(session);
}