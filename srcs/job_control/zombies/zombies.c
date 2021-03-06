/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:19:14 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:39:15 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/conditions.h>
#include <job_control/zombies.h>
#include <job_control/session.h>
#include <signals_print.h>

/*
** Push a node in the zombie processes list using it as a stack.
*/

bool				zombies_list_update(t_group *update)
{
	t_background	*fill;
	t_background	*zombie;

	if (!update || !(zombie = ft_calloc(1, sizeof(t_background))))
		return (false);
	*zombie = (t_background){.background_group=update};
	fill = g_session.zombies;
	g_session.zombies = zombie;
	zombie->next = fill;
	return (true);
}

static void			remove_zombie_node(t_background **prev, t_background **next,
		t_background **first)
{
	if (*prev)
		(*prev)->next = g_session.zombies->next;
	if (*first == g_session.zombies)
		*first = NULL;
	if (*next == g_session.zombies)
		*next = (*next)->next;
	free(g_session.zombies);
	g_session.zombies = NULL;
}

/*
** Removes a node containing the given target as parameter in the
** zombies list using it as a linked-list.
*/

void				zombies_list_remove_node(t_group *target)
{
	t_background	*prev;
	t_background	*next;
	t_background	*first;

	first = g_session.zombies;
	prev = NULL;
	next = NULL;
	if (g_session.zombies)
		next = g_session.zombies->next;
	while (g_session.zombies)
	{
		if (target->nil->next->pid \
			== g_session.zombies->background_group->nil->next->pid)
		{
			remove_zombie_node(&prev, &next, &first);
			break ;
		}
		prev = g_session.zombies;
		g_session.zombies = g_session.zombies->next;
	}
	if (first)
		g_session.zombies = first;
	else
		g_session.zombies = next;
}

/*
** Destroys all the groups nodes containing exited groups in the
** job control data structure.
*/

void				zombies_list_purge_exited_groups(void)
{
	t_group			*first;
	t_group			*next;

	first = g_session.groups;
	while (g_session.groups && g_session.groups != g_session.nil)
	{
		next = g_session.groups->next;
		if (!group_condition(g_session.groups, is_active) \
				&& group_condition(g_session.groups, is_removable))
		{
			if (g_session.groups == first)
				first = first->next;
			history_session_remove_node(g_session.groups);
			group_remove(&g_session.groups);
		}
		g_session.groups = next;
	}
	g_session.groups = first;
}

/*
** Destroy all the zombies nodes in the zombies list containing an exited
** group.
*/

void				zombies_list_purge_exited_zombies(void)
{
	t_background	*next;
	t_background	*first;
	t_background	*prev;

	prev = NULL;
	first = g_session.zombies;
	while (g_session.zombies)
	{
		next = g_session.zombies->next;
		if (g_session.zombies->exited)
		{
			if (first && first == g_session.zombies)
				first = first->next;
			free(g_session.zombies);
			if (prev)
				prev->next = next;
		}
		else
			prev = g_session.zombies;
		g_session.zombies = next;
	}
	g_session.zombies = first;
}
