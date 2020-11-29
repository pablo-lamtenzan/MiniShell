/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helper3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 04:50:26 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 09:15:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>
#include <sys/wait.h>
#include <signal.h>

const char				*get_signal_(int index)
{
	static const char	*signals[33] = {
		"Hangup", "", "Quit", "Illegal instruction", "Trace/breakpoint trap", \
		"Aborted", "Bus error", "Floating point exception", "Killed", \
		"User defined signal 1", "Segmentation fault", "User defined signal 2",\
		"", "Alarm clock", "Terminated", "Stack fault", "", "", "Stopped", \
		"Stopped", "Stopped", "Stopped", "", "CPU time limit exceeded", \
		"File size limit exceeded", "Virtual timer expired", \
		"Profiling timer expired", "I/O possible", "Power failure", \
		"Bad system call", "Done", "Exit", "Running"
	};

	return (signals[index - 1]);
}

int						check_wstatus(t_process *target, int *exit_status)
{
	if (target->flags & BACKGROUND)
		return (33);
	else if (WIFEXITED(target->wstatus))
	{
		if (target->flags & EXITED && target->ret >= 0)
		{
			*exit_status = (unsigned char)target->ret;
			if (*exit_status)
				return (32);
			else
				return (31);
		}
		else if ((*exit_status = WEXITSTATUS(target->wstatus)))
			return (32);
		else
			return (31);
	}
	else if (WIFSTOPPED(target->wstatus))
		return (WSTOPSIG(target->wstatus));
	if (target->flags & SIGNALED)
		*exit_status = 0;
	return (WTERMSIG(target->wstatus));
}

bool					stopped_signal(int signal, bool ignore_tstp)
{
	return (signal == SIGSTOP || (!ignore_tstp && signal == SIGTSTP) \
		|| signal == SIGTTIN || signal == SIGTTOU);
}

bool					stopped_signal_group(t_group *group, bool wcheck)
{
	t_process			*leader;

	leader = group->active_processes;
	while (leader != group->nil)
	{
		if (leader->flags & STOPPED || (wcheck && WIFSTOPPED(leader->wstatus)))
			return (true);
		leader = leader->next;
	}
	return (false);
}
