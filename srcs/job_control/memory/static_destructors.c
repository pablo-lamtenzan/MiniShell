/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_destructors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:34:43 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 00:30:11 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/destructors.h>
#include <job_control/background.h>
#include <job_control/utils.h>
#include <signal_handler.h>
#include <libft.h>

#include <stdlib.h>

/*
** Forces to exit and wait all the backgroud processes, mute the signal
** handlers and proceed to free all the job control ressources.
*/

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
		delete_group_input(&curr);
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

	while ((*group)->active_processes != (*group)->nil)
	{
		fill = (*group)->active_processes;
		if ((*group)->active_processes->data)
		{
			strs_unload((char**)(*group)->active_processes->data);
			(*group)->active_processes->data = NULL;
		}
		(*group)->active_processes = (*group)->active_processes->next;
		free(fill);
		fill = NULL;
	}
}
