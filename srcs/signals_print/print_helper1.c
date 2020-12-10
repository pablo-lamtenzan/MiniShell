/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helper1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 04:41:38 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:30:49 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <signals_print.h>
#include <job_control/conditions.h>

#include <sys/wait.h>
#include <signal.h>

void			print_pid(int fd, t_process *target, int mode)
{
	char		*freed;

	freed = NULL;
	ft_dprintf(fd, "%s", mode ? freed = ft_itoa(target->pid) : "");
	free(freed);
}

void			print_signal_(int fd, t_process *target, int mode, int signal)
{
	t_group		*aux;

	aux = group_get(target);
	ft_dprintf(fd, "%s", (!mode && stopped_signal_group(aux, false) \
	? "Stopped" : get_signal_(signal)));
}

void			print_exit_st(int fd, int exit_st)
{
	char		*freed;

	freed = NULL;
	ft_dprintf(fd, "%s%s",
		exit_st > 0 ? " " : "", exit_st > 0 ? freed = ft_itoa(exit_st) : "");
	free(freed);
}

void			print_coredump(int fd, t_process *target, int mode)
{
	t_group		*aux;

	aux = group_get(target);
	ft_dprintf(fd, "%s",
		(mode && WCOREDUMP(target->wstatus)) \
		|| (!mode && group_condition(aux, is_coredump)) ? "(core dumped)" : "");
}

void			print_sp(int fd, t_process *target, int mode, int exit_st)
{
	const bool	is__leader = is_leader(target);
	t_group		*aux;

	aux = group_get(target);
	ft_dprintf(fd, "%s",
		ft_norme_makes_my_code_worst((stopped_signal_group(aux, true) \
			|| exit_st >= 0) && (!mode || (mode && is__leader)), " ", ""));
}
