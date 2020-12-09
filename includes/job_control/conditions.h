/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:55:05 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:52:35 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONDITIONS_H
# define CONDITIONS_H

/*
** Local
*/
# include <job_control/process.h>

bool					is_active(t_process *target);
bool					is_exited(t_process *target);
bool					is_removable(t_process *target);
bool					is_coredump(t_process *target);
bool					is_signaled(t_process *target);
bool					is_stopped(t_process *target);

#endif
