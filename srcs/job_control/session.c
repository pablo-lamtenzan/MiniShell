/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:26:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 23:56:37 by pablo            ###   ########.fr       */
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
	//if (PRINT_DEBUG)
		ft_dprintf(2, "[NEW SESSION][\'%p\'][NIL=\'%p\']\n", g_session, g_session->nil);
	return (g_session);
}

void		session_end()
{
	force_exit_background();
	delete_groups();
	delete_endzombies();
	delete_zombies();
	delete_hist();
	//delete_input_line();
	free(g_session->nil);
	free(g_session);
	g_session = NULL;
}

void		delete_groups()
{
	t_group*		fill;
	while (g_session->groups != g_session->nil)
	{
		fill = g_session->groups;
		g_session->groups = g_session->groups->next;
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

	while (g_session->zombies)
	{
		fill = g_session->zombies;
		g_session->zombies = g_session->zombies->next;
		free(fill);
		fill = NULL;
	}
}

void		delete_hist()
{
	t_history*	fill;

	while (g_session->hist)
	{
		fill = g_session->hist;
		g_session->hist = g_session->hist->next;
		free(fill);
		fill = NULL;
	}
}

void		delete_input_line()
{
	int		i;

	i = -1;
	while (g_session->input_line && g_session->input_line[++i])
		free(g_session->input_line[i]);
	free(g_session->input_line);
	g_session->input_line = NULL;
}