/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 18:03:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 23:18:29 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

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

/* TO USE WHEN SESSION WILL BE GLOBAL IN: fg, bg, kill, disown, jobs */

t_session* session;

void				for_each_process_in_group(t_process* target_leader, t_group* itself, int arg, void (*action)(t_group**, t_process**, int))
{
	const t_group*	remember = session->groups;
	t_group*		fill;
	t_process*		remember_leader;

	if (session->groups == itself)
		session->groups = session->groups->next;
	while (session->groups != session->nil)
	{
		if (session->groups->nil->next->pid == target_leader->pid)
		{
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				action(&session->groups, &session->groups->active_processes, arg);
				session->groups->active_processes = session->groups->active_processes->next;
			}
			if (!is_active_group(session->groups))
			{
				fill = session->groups;
				session->groups->prev->next = session->groups->next;
				session->groups->next->prev = session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				session->groups->active_processes = remember_leader;
			session->groups = (t_group*)remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = (t_group*)remember;
}

/* SAME */

void			for_each_group(int arg, void (*action)(t_group**, t_process**, int))
{
	const t_group*	remember = session->groups;
	
	session->groups = session->nil->prev;
	while (session->groups != session->nil->next) // != itself
	{
		for_each_process_in_group(session->groups->nil->next, (t_group*)remember, arg, action);
		session->groups = session->groups->prev;
	}
	session->groups = (t_group*)remember;
}