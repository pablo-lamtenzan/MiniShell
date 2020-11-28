/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:20:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 23:53:41 by pablo            ###   ########.fr       */
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
	if ((*target)->flags & EXITED)
		return (WEXITSTATUS((*target)->wstatus));
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, "bash: warning: wait_for_job: job %lu[%d] is stopped\n", \
				background_index_get(g_session->nil, *target), (*target)->pid);
		return (148);
	}
	background_update(target);
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, "bash: warning: wait_for_job: job %lu[%d] is stopped\n", \
				background_index_get(g_session->nil, *target), (*target)->pid);
		return (CMD_NOT_FOUND);
	}
	// TO DO: PRINTS THE INPUT CMD
	print_signal(2, *target, STANDART);
	return (WEXITSTATUS((*target)->wstatus));
}

bool			wait_delete()
{
	t_group		*next;

	if (!group_condition(g_session->groups, is_active))
	{
		zombies_list_remove_node(g_session->groups);
		history_session_remove_node(g_session->groups);
		next = g_session->groups->next;
		g_session->groups->prev->next = next;
		next->prev = g_session->groups->prev;
		free(g_session->groups);
		g_session->groups = next;
		return (true);
	}
	return (false);
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
				ret = wait_process(&g_session->groups->active_processes, flags);
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			if (!wait_delete())
				g_session->groups->active_processes = remember_leader;
			break ;
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
		if (g_session->groups->nil->next->flags & (SIGNALED | KILLED))
		{
			g_session->groups = prev;
			continue ;
		}
		ret = wait_group(g_session->groups->nil->next, flags, remember);
		g_session->groups = prev;
	}
	g_session->groups = remember;
	return (ret);
}

int			wait_init_exeption(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "nf", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: wait: %s: inalid option\n%s\n", args->av[1], \
				"wait: usage: wait [-fn] [id ...]");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			if (!is_string_digit(args->av[1]))
				ft_dprintf(2, "bash: wait: `%s\': not a pid or valid job spec\n", \
						args->av[1]);
			else
				ft_dprintf(STDERR_FILENO, "minish: wait: pid %s: is not a child of this shell\n", \
						args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

int			wait_jobspec(t_exec *args, int nb, int i, t_process*** target)
{
	if (!(*target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
	{
		if (is_string_digit(args->av[nb + i + 1]))
			ft_dprintf(STDERR_FILENO, "minsh: wait: pid %s is not a child of this shell\n", \
					args->av[nb + i + 1]);
		else
			ft_dprintf(STDERR_FILENO, "minish: wait: %s: no such job\n", \
					args->av[nb + i + 1]);
		return (CMD_NOT_FOUND);
	}
	return (42);
}

// TO DO RM I WHEN I CAN TEST
int			ft_wait(t_exec* args)
{
	int flags;
	int i;
	static int last_return = SUCCESS;
	int			tmp;
	t_process**	target;
	int nb;

	flags = 0;
	if ((i = wait_init_exeption(args, &flags, &nb)) != 42)
		return (i);
	i = -1;
	flags = flags < 0 ? -flags : flags;
	if (args->ac > 1 && args->ac > nb + 1 && ++i < args->ac - nb - 1)
	{
		if ((tmp = wait_jobspec(args, nb, i, &target)) != 42)
			return (tmp);
		tmp = last_return;
		last_return = wait_group(*target, flags, g_session->groups);
		return (!flags ? tmp : last_return);
	}
	wait_all_groups(flags);
	return (SUCCESS);
}