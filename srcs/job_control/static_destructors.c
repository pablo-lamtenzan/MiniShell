/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_destructors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:34:43 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:08:17 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void		session_end()
{
	background_force_exit();
	delete_groups();
	//delete_endzombies();
	delete_zombies();
	delete_hist();
	//delete_input_line();
	free(g_session.nil);
}

void		delete_groups()
{
	t_group*		fill;
	while (g_session.groups != g_session.nil)
	{
		fill = g_session.groups;
		g_session.groups = g_session.groups->next;
		delete_processes(fill);
		free(fill->nil);
		free(fill);
		fill = NULL;
	}
}

void		delete_processes(t_group* group)
{
	t_process*	fill;
	int			i;

	i = -1;
	while (group->active_processes != group->nil)
	{
		fill = group->active_processes;
		while (group->active_processes->data && group->active_processes->data[++i])
			free(group->active_processes->data[i]);
		free((char**)group->active_processes->data);
		group->active_processes = group->active_processes->next;
		free(fill);
		fill = NULL;
	}
}

void		delete_zombies()
{
	t_background*	fill;

	while (g_session.zombies)
	{
		fill = g_session.zombies;
		g_session.zombies = g_session.zombies->next;
		free(fill);
		fill = NULL;
	}
}

void		delete_hist()
{
	t_history*	fill;

	while (g_session.hist)
	{
		fill = g_session.hist;
		g_session.hist = g_session.hist->next;
		free(fill);
		fill = NULL;
	}
}

/* 6 fcts  put this in session end */
void		delete_input_line()
{
	int		i;

	i = -1;
	while (g_session.input_line && g_session.input_line[++i])
		free(g_session.input_line[i]);
	free(g_session.input_line);
	g_session.input_line = NULL;
}