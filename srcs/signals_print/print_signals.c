/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 21:45:15 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:30:38 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <signals_print.h>
#include <job_control/background.h>
#include <job_control/utils.h>
#include <job_control/conditions.h>

#include <sys/wait.h>
#include <signal.h>

void			print_job_args(int fd, t_process *target)
{
	int			i;

	i = -1;
	while (target && target->data && target->data[++i])
		ft_dprintf(fd, "%s%s", target->data[i], \
			target->data[i + 1] ? " " : "");
}

void			print_index_args(t_process *target)
{
	const bool	leader = is_leader(target);
	char		*index;

	index = NULL;
	ft_dprintf(STDERR_FILENO, "%s%s%s%-2s",
		leader ? "[" : " ",
		leader ? index = \
		ft_itoa(background_index_get(g_session.nil, target)) : " ",
		leader ? "]" : " ",
		is_in_history(target));
	print_job_args(2, target);
	free(index);
}

void			print_signal(int fd, t_process *target, int mode)
{
	int			signal;
	int			exit_status;
	t_group		*aux;

	aux = group_get(target);
	exit_status = -1;
	if (stopped_signal(signal = check_wstatus(target, &exit_status), true))
		write(fd, "\n", 1);
	print_index(fd, target, mode, exit_status);
	print_history(fd, target, mode, exit_status);
	print_sp(fd, target, mode, exit_status);
	print_pid(fd, target, mode);
	print_sp(fd, target, mode, exit_status);
	print_signal_(fd, target, mode, signal);
	print_exit_st(fd, exit_status);
	print_coredump(fd, target, mode);
	if (stopped_signal(signal, false) || signal == 33 || signal == 31 \
			|| target->flags & SIGNALED || exit_status > 0)
	{
		padding_spaces(fd, ft_strlen((!mode \
		&& group_condition(aux, is_active) ? "Stopped" : get_signal_(signal))));
		mode ? print_job_args(fd, target) : print_group_line(fd, aux);
	}
	write(fd, "\n", 1);
}
