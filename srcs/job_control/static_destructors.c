/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_destructors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:34:43 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 07:11:34 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void				session_end()
{
	g_session.flags = 0;
	background_force_exit();
	delete_groups();
	delete_deadzombies();
	delete_zombies();
	delete_hist();
	delete_input_line();
	free(g_session.nil);
}

void				delete_deadzombies()
{
	t_deadzombie	*next;

	while (g_session.dead_zombies)
	{
		next = g_session.dead_zombies->next;
		free(g_session.dead_zombies);
		g_session.dead_zombies = next;
	}
}

void				delete_groups()
{
	t_group			*fill;
	while (g_session.groups != g_session.nil)
	{
		fill = g_session.groups;
		g_session.groups = g_session.groups->next;
		delete_group_input(&g_session.groups);
		delete_processes(&fill);
		free(fill->nil);
		free(fill);
		fill = NULL;
	}
}

void				delete_group_input(t_group **group)
{
	int				i;
	
	i = -1;
	while ((*group)->input && (*group)->input[++i])
		free((*group)->input[i]);
	free((*group)->input);
	(*group)->input = NULL;
}

void				delete_processes(t_group **group)
{
	t_process		*fill;
	int				i;

	i = -1;
	while ((*group)->active_processes != (*group)->nil)
	{
		fill = (*group)->active_processes;
		while ((*group)->active_processes->data && ++i < matrix_height(((char**)(*group)->active_processes->data)))
			free((*group)->active_processes->data[i]);
		free((char**)(*group)->active_processes->data);
		(*group)->active_processes->data = NULL;
		(*group)->active_processes = (*group)->active_processes->next;
		free(fill);
		fill = NULL;
	}
}
