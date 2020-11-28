/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 03:28:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 22:26:55 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

/*
** External
*/
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>

/*
** Local
*/
# include <job_control.h>
# include <libft.h>

/*
** Modes
*/
# define STANDART		0
# define ADVANCED		1

# define SIGNAL_PADDING 18

/*
** Helpers
*/
void					print_index_args(t_process* target);
void					print_job_args(int fd, t_process* target);
const char				*get_signal(int index);
int						check_wstatus(t_process* target, int *exit_status);
bool					stopped_signal(int signal, bool ignore_tstp);
bool					stopped_signal_group(t_group *group, bool wcheck);
void					padding_spaces(int fd, size_t alreaddy_written);
void					print_group_line(int fd, t_group *group);
char					*ft_norme_makes_my_code_worst(bool condition, char *ret_true, char *ret_false);

/*
** Printers
*/
void					print_index(int fd, t_process *target, int mode, int exit_st);
void					print_history(int fd, t_process *target, int mode, int exit_st);
void					print_sp(int fd, t_process *target, int mode, int exit_st);
void					print_pid(int fd, t_process *target, int mode);
void					print_signal_(int fd, t_process *target, int mode, int signal);
void					print_exit_st(int fd, int exit_st);
void					print_coredump(int fd, t_process* target, int mode);

/*
** Global printer
*/
void					print_signal(int fd, t_process* target, int mode);

#endif
