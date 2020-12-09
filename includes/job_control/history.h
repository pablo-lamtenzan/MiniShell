/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:30:54 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:53:22 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

/*
** Local
*/
# include <job_control/group.h>

typedef struct			s_history
{
	t_group				*group;
	struct s_history	*next;
}						t_history;

bool					history_session_update(t_group *update);
void					history_pop_front();
void					history_session_remove_node(t_group *target);
void					history_session_purge_exited();

#endif
