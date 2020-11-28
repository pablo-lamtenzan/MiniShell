/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:48:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 21:59:55 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

void			disown_process(t_process** target, int flags)
{
	int i;

	i = -1;
	if (flags & 1 && (*target)->flags & STOPPED)
	{
		(*target)->flags |= RESTRICT_OP;
		history_pop_front();
		return ;
	}
	if (flags & 4 && ((*target)->flags |= NO_HANGUP))
		return ;
	if ((*target)->flags & STOPPED)
		ft_dprintf(STDERR_FILENO, \
		"minish: warning: deleting stopped job %lu with process group %d\n", \
			background_index_get(g_session->nil, *target), \
			process_get_leader_pid(g_session->nil, *target));
	remove_process(target);
}

int		disowm_delete()
{
	t_group*	fill;
	if (!group_condition(g_session->groups, is_active))
	{
		history_session_remove_node(g_session->groups);
		fill = g_session->groups;
		g_session->groups->prev->next = g_session->groups->next;
		g_session->groups->next->prev = g_session->groups->prev;
		free(fill);
		fill = NULL;
		return (true);
	}
	return (false);
}

void		disown_core(int flags)
{
	t_process*	next;
	t_process*	remember_leader;

	remember_leader = g_session->groups->active_processes;
	while (g_session->groups->active_processes != g_session->groups->nil)
	{
		next = g_session->groups->active_processes->next;
		disown_process(&g_session->groups->active_processes, flags);
		g_session->groups->active_processes = next;
	}
	if (!disowm_delete())
		g_session->groups->active_processes = remember_leader;
}

void		disown_group(t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	

	remember = g_session->groups;
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			disown_core(flags);
			break ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

void		disown_all_groups(int flags)
{
	t_group*	remember;
	t_group*	prev;

	remember = g_session->groups;

	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		prev = g_session->groups->prev;
		disown_group(g_session->groups->nil->next, flags, remember);
		g_session->groups = prev;
	}
	g_session->groups = remember;
}

int		disown_init_exeption(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "rah", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s", \
			"minish: usage: disown: [-h] [-ar] [jobspec ... | pid ...]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

int		disown_jobspec(t_exec *args, int flags, int nb, int i)
{
	t_process** target;

	while (++i < args->ac - nb - 1)
	{
		if (!(target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
		{
			ft_dprintf(STDERR_FILENO, "minish: disown: %s: no such job\n", \
				args->av[nb + i + 1]);
			return (STD_ERROR);
		}
		disown_group(*target, flags, g_session->groups);
	}
	return (42);
}

int		ft_disown(t_exec* args)
{
	int	flags;
	int i;
	int	exept;
	int nb;

	flags = 0;
	if ((exept = disown_init_exeption(args, &flags, &nb)) != 42)
		return (exept);
	i = -1;
	if (args->ac > 1)
	{
		if ((flags < 0 || !(flags & 2)) \
			&& (exept = disown_jobspec(args, flags < 0 \
			? -flags : flags, nb, i)) != 42)
			return (exept);
		else
			disown_all_groups(flags < 0 ? 0 : flags);
	}
	else if (g_session->hist)
		disown_group(g_session->hist->group->nil->next, \
			flags < 0 ? -flags : flags, g_session->groups);
	return (SUCCESS);
}