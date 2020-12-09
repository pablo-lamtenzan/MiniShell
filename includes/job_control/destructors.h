/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:50:06 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:52:39 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef DESTRUCTORS_H
# define DESTRUCTORS_H

/*
** Local
*/
# include <job_control/group.h>

void					delete_groups();
void					delete_processes(t_group **group);
void					delete_zombies();
void					delete_deadzombies();
void					delete_hist();
void					delete_input_line();
void					delete_group_input(t_group **group);

#endif
