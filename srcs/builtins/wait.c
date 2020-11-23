/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:20:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 09:15:16 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signals.h>
#include <job_control.h>

/* TESTS

1) Wait without args DONE
2) Wait in backgound process DONE
3) Wait for pipe DONE
4) Wait for multiple groups DONE
5) Wait bad flag DONE
6) Wait invalid jobspec DONE

*/

int			wait_process(t_process** target, int flags)
{
	(void)flags;
	// -n RETURN CURR GROUP WAITED EXIT STATUS

	// -f WAIT FOR ALL (ignore -f per the moment)

	// no flag return prev exit status

	// warning if process is stopped
	if ((*target)->flags & EXITED)
		return (WEXITSTATUS((*target)->wstatus));
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, "bash: warning: wait_for_job: job %lu[%d] is stopped\n", get_background_index(g_session->nil, *target), (*target)->pid);
		return (148);
	}
	update_background(target, true);
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, "bash: warning: wait_for_job: job %lu[%d] is stopped\n", get_background_index(g_session->nil, *target), (*target)->pid);
		return (CMD_NOT_FOUND);
	}
	// TO DO: PRINTS THE INPUT CMD
	print_signal_v2(*target, 2);
	return (WEXITSTATUS((*target)->wstatus));
}

int			wait_group(t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;
	int ret;

	ret = 0;
	remember = g_session->groups;

	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;

	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				if (PRINT_DEBUG){
				ft_dprintf(2, "[WAIT][TARGET FLAGS: %d][\'%p\']\n", g_session->groups->active_processes->flags, g_session->groups->active_processes);
				ft_dprintf(2, "[WAIT][PROCESS: %d][\'%p\']\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);}
				ret = wait_process(&g_session->groups->active_processes, flags);
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			if (!is_active_group(g_session->groups))
			{
				t_group*	fill = g_session->groups;
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return (ret);
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
	return (ret);
}

int			wait_all_groups(int flags)
{
	t_group*	remember;
	t_group*	prev;
	int			ret;

	ret = 0;
	remember = g_session->groups;

	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		prev = g_session->groups->prev;
		ret = wait_group(g_session->groups->nil->next, flags, remember);
		g_session->groups = prev;
	}
	g_session->groups = remember;
	return (ret);
}

int			ft_wait(t_exec* args, t_term* term)
{
	/*
	wait [-fn] [jobspec or pid]:
 -> Wait until the child process (jobspec or pid) exits
 -> Returns the exit status of the last command waited for
 -> If a jobspec is given, all the processes in the job are waited for
 -> No options, all currently active childs are waited, returns 0
 -> '-n' wait for a single job and return its exit status 
 -> '-f' forces to wait for each job to terminate before returning its status intead of returning when it changes status
 -> Job not found return 127
 -> At the end when a process exit prints per ex: "[1]-  Done                    sleep 22"
 */
	(void)term;
	int flags;
	int i;
	static int last_return = SUCCESS;
	int			tmp;
	t_process**	target;

	flags = 0;
	i = -1;
	if ((flags = parse_flags(args->ac, args->av[1], "nf")) < 0 && args->av[1][0] == '-') // lexer error flags
	{
		ft_dprintf(STDERR_FILENO, "minish: wait: %s: inalid option\n%s\n", args->av[1], "wait: usage: wait [-fn] [id ...]");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		ft_dprintf(2, "[WAIT FLAGS][%d]\n", flags);
		if (args->ac > 1 && flags < 0)
		{
			if (!is_string_digit(args->av[1]))
				ft_dprintf(2, "bash: wait: `%s\': not a pid or valid job spec\n", args->av[1]);
			else
				ft_dprintf(STDERR_FILENO, "minish: wait: pid %s: is not a child of this shell\n", args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	if (args->ac > 1)
	{

		if (args->ac >= 2)
		{
			while (++i < args->ac - (flags > 0 ? 2 : 1))
			{
				if (!(target = jobspec_parser(args->ac, &args->av[flags > 0 ? 1 : 0], NULL)))
				{
					// TO DO ERROR PID
					ft_dprintf(STDERR_FILENO, "bash: wait: %s: no such job\n", args->av[flags > 0 ? 1 : 0]);
					return (CMD_NOT_FOUND);
				}
				tmp = last_return;
				last_return = wait_group(*target, flags < 0 ? 0 : flags, g_session->groups);
				return (!flags ? tmp : last_return);
			}
		}
	}
	wait_all_groups(flags < 0 ? 0 : flags);
	return (SUCCESS);
}