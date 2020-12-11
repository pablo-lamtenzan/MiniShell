/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:44:30 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:52:32 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKGROUND_H
# define BACKGROUND_H

/*
** Local
*/
# include <job_control/session.h>

/*
** Background
*/
void					background_update(t_process **target);
size_t					background_index_get(t_group *nil, t_process *target);
void					background_force_exit();
bool					is_background_stopped();
t_process				**background_find(t_process *target,
						const char *search_type, t_group	*group);

/*
** Exit helper
*/
void					handle_exit_with_active_background(int exit_status,
						t_session *session);
void					update_exit_count(const char *name);

#endif
