/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:29:28 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:53:11 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIES_H
# define ZOMBIES_H

/*
** Local
*/
# include <job_control/group.h>

typedef struct			s_background
{
	t_group				*background_group;
	bool				exited;
	struct s_background	*next;
}						t_background;

typedef struct			s_deadzombie
{
	t_process			*deadzombie;
	struct s_deadzombie	*next;
}						t_deadzombie;

/*
** Zombies list
*/
bool					zombies_list_update(t_group	*update);
void					zombies_list_remove_node(t_group *target);
void					zombies_list_purge_exited_groups();
void					zombies_list_purge_exited_zombies();

/*
** Dead zombies list
*/
t_deadzombie			*deadzombie_new(t_process *target);
bool					deadzombie_push_back(t_deadzombie *target);
void					deadzombies_print();
void					deadzombie_remove_node(t_process *target);

#endif
