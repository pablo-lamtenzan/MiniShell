/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:26:09 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:53:33 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GROUP_H
# define GROUP_H

/*
** Local
*/
# include <job_control/process.h>

typedef struct			s_group
{
	t_process			*active_processes;
	t_process			*nil;
	struct s_group		*next;
	struct s_group		*prev;
	char				**input;
}						t_group;

/*
** Group
*/
t_group					*group_new();
bool					group_empty(t_group *group);
void					group_insert(t_group *prev, t_group *next,
						t_group *target);
void					group_remove(t_group **target);
void					group_push_front(t_group *target);
void					group_push_back(t_group *target);
void					group_pop_front();
void					group_pop_back();
bool					group_condition(t_group *target,
						bool (*condition)(t_process*));
t_group					*group_get(t_process *target);
void					group_return_handler();

/*
** Process
*/
void					process_push_front(t_group **group, t_process *target);
void					process_push_back(t_group **group, t_process *target);
void					process_pop_font(t_group **group);
void					process_pop_back(t_group **group);
pid_t					process_get_leader_pid(t_group *nil, t_process *target);

#endif
