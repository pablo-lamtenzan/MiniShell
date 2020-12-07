/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 18:03:18 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:33:26 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

int				parse_flags(int ac, char*const *av, const char *pattern,
		int *nb_flags)
{
	int			vars[4];

	*nb_flags = 0;
	vars[0] = -1;
	vars[1] = 0;
	while (++vars[0] < ac - 1)
	{
		vars[2] = -1;
		while (av[vars[0]][++vars[2]])
		{
			if (av[vars[0]][vars[2]] == '-')
			{
				if (vars[2] == 0)
					continue ;
				else
					return (-1);
			}
			vars[3] = vars[1];
			if ((vars[1] |= (1 << ft_strpos(pattern, \
					av[vars[0]][vars[2]]))) < 0)
				return (!*nb_flags ? -1 : -vars[3]);
		}
		(*nb_flags)++;
	}
	return (vars[1]);
}

bool			ignore_pid(int ac, char*const *av)
{
	(void)ac;
	if (is_string_digit(av[1]))
		return (false);
	return (true);
}

const char		*is_in_history(t_process *target)
{
	if (!g_session.hist)
		return (" ");
	if (background_find(target, "PID", g_session.hist->group) \
			&& is_leader(target))
		return ("+");
	else if (g_session.hist->next \
			&& background_find(target, "PID", g_session.hist->next->group) \
			&& is_leader(target))
		return ("-");
	else
		return (" ");
}

static void		for_each_process_loop(int *ret, int (*core)())
{
	while (g_session.groups->active_processes \
				!= g_session.groups->nil)
	{
		*ret = core();
		g_session.groups->active_processes = \
		g_session.groups->active_processes->next;
	}
}

int				for_each_in_group(t_process *leader,
		int (*core)(), bool (*delete)())
{
	t_group		*remember;
	t_process	*remember_leader;
	int			ret;

	ret = SUCCESS;
	remember = g_session.groups;
	g_session.groups = g_session.groups->next;
	while (g_session.groups != g_session.nil)
	{
		if (g_session.groups->active_processes->pid == leader->pid)
		{
			remember_leader = g_session.groups->active_processes;
			for_each_process_loop(&ret, core);
			if ((delete && !delete()) || !delete)
				g_session.groups->active_processes = remember_leader;
			break ;
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = remember;
	return (ret);
}
