/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dead_zombies.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 04:21:23 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:45:50 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <signals_print.h>

/*
** Push a new node using the zombie list ass a queue.
*/

bool				deadzombie_push_back(t_deadzombie *target)
{
	t_deadzombie	*remember;

	if (!target)
		return (false);
	if (!(remember = g_session.dead_zombies))
	{
		g_session.dead_zombies = target;
		return (true);
	}
	while (g_session.dead_zombies->next)
		g_session.dead_zombies = g_session.dead_zombies->next;
	g_session.dead_zombies->next = target;
	g_session.dead_zombies = remember;
	return (true);
}

/*
** Print the data of the zombies when they exit, then the
** nodes are destroyed.
*/

void				deadzombies_print(void)
{
	t_deadzombie	*next;

	while (g_session.dead_zombies)
	{
		next = g_session.dead_zombies->next;
		g_session.dead_zombies->deadzombie->flags &= ~NO_DELETE;
		print_signal(STDERR_FILENO, g_session.dead_zombies->deadzombie, \
			STANDART);
		free(g_session.dead_zombies);
		g_session.dead_zombies = next;
	}
	g_session.dead_zombies = NULL;
}

static void			remove_dead_node(t_deadzombie **prev, t_deadzombie **next,
		t_deadzombie **first)
{
	if (*prev)
		(*prev)->next = g_session.dead_zombies->next;
	if (*first == g_session.dead_zombies)
		*first = NULL;
	if (*next == g_session.dead_zombies)
		*next = (*next)->next;
	free(g_session.dead_zombies);
	g_session.dead_zombies = NULL;
}

/*
** Remove the dead zombie node in the dead zombie list who has
** the same pid as the target param
*/

void				deadzombie_remove_node(t_process *target)
{
	t_deadzombie	*prev;
	t_deadzombie	*next;
	t_deadzombie	*first;

	first = g_session.dead_zombies;
	prev = NULL;
	next = NULL;
	if (g_session.dead_zombies)
		next = g_session.dead_zombies->next;
	while (g_session.dead_zombies)
	{
		if (target->pid == g_session.dead_zombies->deadzombie->pid)
		{
			remove_dead_node(&prev, &next, &first);
			break ;
		}
		prev = g_session.dead_zombies;
		g_session.dead_zombies = g_session.dead_zombies->next;
	}
	if (first && first->deadzombie && first->deadzombie->pid \
			== target->pid)
		g_session.dead_zombies = first->next;
	else
		g_session.dead_zombies = next;
}
