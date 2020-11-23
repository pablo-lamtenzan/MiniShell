/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 18:03:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 08:37:21 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

int				parse_flags(int ac, const char* av, const char* pattern)
{
	int			cont;
	int			flags;
	int 		i;

	cont = -1;
	flags = 0;
	while (++cont < ac - 1)
	{
		i = -1;
		while (av[++i])
		{
			if (av[i] == '-')
			{
				if (i == 0)
					continue ;
				else
					return (-1);
			}
			if ((flags |= (1 << ft_strpos(pattern, av[i]))) < 0)
				return (-1);
			
		}
	}
	return (flags);
}

/* TO USE WHEN g_session WILL BE GLOBAL IN: fg, bg, kill, disown, jobs */

void				for_each_process_in_group(t_process* target_leader, t_group* itself, int arg, void (*action)(t_group**, t_process**, int))
{
	const t_group*	remember = g_session->groups;
	t_group*		fill;
	t_process*		remember_leader;

	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == target_leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				action(&g_session->groups, &g_session->groups->active_processes, arg);
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			if (!is_active_group(g_session->groups))
			{
				fill = g_session->groups;
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				g_session->groups->active_processes = remember_leader;
			g_session->groups = (t_group*)remember;
			return ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = (t_group*)remember;
}

/* SAME */

void			for_each_group(int arg, void (*action)(t_group**, t_process**, int))
{
	const t_group*	remember = g_session->groups;
	
	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next) // != itself
	{
		for_each_process_in_group(g_session->groups->nil->next, (t_group*)remember, arg, action);
		g_session->groups = g_session->groups->prev;
	}
	g_session->groups = (t_group*)remember;
}

int				builtin_error(size_t index, int ret)
{
	(void)index;
	// TO DO: search by index in errors database
	
	// AND

	// returns the error
	return (ret);
}