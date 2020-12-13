/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_builtins.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:46:11 by pablo             #+#    #+#             */
/*   Updated: 2020/12/13 03:23:41 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef JC_BUILTINS_H
# define JC_BUILTINS_H

/*
** Local
*/
# include <job_control/group.h>

void					keep_alive_killed_processes();
void					print_process(int fd, t_process *target, int flags);
bool					is_not_running(t_process *target);
bool					is_not_stopped(t_process *target);
void					print_group(int fd, t_process *leader, int flags,
						t_group *itself);
int						wait_process(t_process **target, int flags);
int						wait_group(t_process *leader, int flags,
						t_group *itself);
int						wait_all_groups(int flags);
const char				*get_signal(const char *key, int *res);
void					kill_core(int signal);
int						handle_current(t_process ***target,
						const char *jobspec);
void					kill_group(t_process *leader, int signal,
						t_group *itself);
void					print_all_signals();
void					disown_process(t_process **target, int flags);
int						disowm_delete();
void					disown_core(int flags);
void					disown_all_groups(int flags);
void					disown_group(t_process *leader, int flags,
						t_group *itself);

#endif
