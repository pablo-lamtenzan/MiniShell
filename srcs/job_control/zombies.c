/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:19:14 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 21:55:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <signals.h>

/*
replaces update_zombies
*/
bool				zombies_list_update(t_group	 *update)
{
	t_background	*fill;
	t_background	*zombie;

	if (!update || !(zombie = ft_calloc(1, sizeof(t_background))))
		return (false);
	*zombie = (t_background){.background_group=update};
	fill = g_session->zombies;
	g_session->zombies = zombie;
	zombie->next = fill;
	return (true);
}

/*
replaces
remove_zombie_node not used i think
*/
/*
void				zombies_list_node_remove(t_group *target)
{
	t_background	*prev;
	t_background	*first;

	first = g_session->zombies;
	prev = NULL;
	if (!target)
		return ;
	while (g_session->zombies)
	{
		if (*g_session->zombies->background_group && target->nil->next->pid \
				== (*g_session->zombies->background_group)->nil->next->pid)
		{
			if (prev)
				prev->next = g_session->zombies->next;
			first = first == g_session->zombies ? NULL : first;
			free(g_session->zombies);
			break ;
		}
		prev = g_session->zombies;
		g_session->zombies = g_session->zombies->next;
	}
	if (first && *first->background_group && \
		(*first->background_group)->nil->next->pid == target->nil->next->pid)
		g_session->zombies = first->next;
	else
		g_session->zombies = first;
}
*/

/*
replaces remove_exited_zombies
*/
void			zombies_list_purge_exited_groups()
{
	t_group		*first;
	t_group		*next;

	first = g_session->groups;
	while (g_session->groups && g_session->groups != g_session->nil)
	{
		next = g_session->groups->next;
		
		if (!group_condition(g_session->groups, is_active) \
				&& group_condition(g_session->groups, is_removable))
		{
			if (g_session->groups == first)
				first = first->next;
			history_session_remove_node(g_session->groups);
			group_remove(&g_session->groups);
		}
		g_session->groups = next;
	}
	g_session->groups = first;
}

void		deadzombie_push_back(t_deadzombie* target)
{
	t_deadzombie*	remember;

	if (!(remember = g_session->dead_zombies))
	{
		g_session->dead_zombies = target;
		return ;
	}
	while (g_session->dead_zombies->next)
		g_session->dead_zombies = g_session->dead_zombies->next;
	g_session->dead_zombies->next = target;
}

void		deadzombies_print()
{
	t_deadzombie*	next;

	while (g_session->dead_zombies)
	{
		next = g_session->dead_zombies->next;
		g_session->dead_zombies->deadzombie->flags &= ~NO_DELETE;
		print_signal(STDERR_FILENO, g_session->dead_zombies->deadzombie, STANDART);
		free(g_session->dead_zombies);
		g_session->dead_zombies = next;
	}
	g_session->dead_zombies = NULL;
}

/*
replaces rm_end_zombies
*/
void		zombies_list_purge_exited_zombies()
{
	t_background*	next;
	t_background*	first;
	t_background*	prev;
	bool			freed;

	prev = NULL;
	freed = false;
	first = g_session->zombies;
	while (g_session->zombies)
	{
		if (g_session->zombies->exited)
		{
			if (first && first == g_session->zombies)
				first = first->next;
			next = g_session->zombies->next;
			// TO DO: test remove history node here
			free(g_session->zombies);
			g_session->zombies = next;
			if (prev)
				prev->next = g_session->zombies;
		}
		prev = g_session->zombies;
		if (!freed && g_session->zombies)
			g_session->zombies = g_session->zombies->next;
	}
	g_session->zombies = first;
}