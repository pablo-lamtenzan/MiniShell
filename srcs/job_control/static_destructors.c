/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_destructors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:34:43 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 22:17:07 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void				session_end(t_session *sess)
{
	background_force_exit(sess);
	ignore_all_signals();
	delete_groups(sess);
	delete_deadzombies(sess);
	delete_zombies(sess);
	delete_hist(sess);
	delete_input_line(sess);
	env_clr(&sess->env);
	free(sess->name);
	free(sess->nil);
	sess->name = NULL;
	sess->nil = NULL;
}

void				delete_deadzombies(t_session *sess)
{
	t_deadzombie	*curr;

	while ((curr = sess->dead_zombies))
	{
		sess->dead_zombies = curr->next;
		free(curr);
	}
}

void				delete_groups(t_session *sess)
{
	t_group			*curr;

	while ((curr = sess->groups) != sess->nil)
	{
		sess->groups = curr->next;
		delete_group_input(&sess->groups);
		delete_processes(&curr);
		free(curr->nil);
		free(curr);
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
		while ((*group)->active_processes->data && ++i < \
				matrix_height(((char**)(*group)->active_processes->data)))
			free((*group)->active_processes->data[i]);
		free((char**)(*group)->active_processes->data);
		(*group)->active_processes->data = NULL;
		(*group)->active_processes = (*group)->active_processes->next;
		free(fill);
		fill = NULL;
	}
}
