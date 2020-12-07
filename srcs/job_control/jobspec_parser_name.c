/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser_name.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:49:49 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:35:45 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

static bool		cond1(t_group *groups, const char *av)
{
	return (!ft_strncmp(\
		groups->active_processes->data[0], av, \
		ft_strlen(groups->active_processes->data[0])) \
		&& is_leader(groups->active_processes) \
		&& is_not_ambigous(groups->active_processes));
}

static bool		cond2(t_group *groups, const char *av, int count)
{
	return (ft_strnstr(groups->active_processes->data[count], \
		&av[1], ft_strlen(&av[1])) \
		&& is_leader(groups->active_processes) \
		&& is_not_ambigous_v2(&av[1]));
}

t_process		**get_process_by_name(t_group *groups, const char *av)
{
	const int	search_mode = get_search_mode(av);
	int			count;

	if (search_mode == 0)
		return (NULL);
	while (groups != g_session.nil)
	{
		while (groups->active_processes != groups->nil)
		{
			count = -1;
			if (search_mode == 1 && cond1(groups, av))
				return (&groups->active_processes);
			else
			{
				while (++count < \
					matrix_height((char**)groups->active_processes->data))
					if (cond2(groups, av, count))
						return (&groups->active_processes);
			}
			groups->active_processes = groups->active_processes->next;
		}
		groups = groups->next;
	}
	return (NULL);
}

bool			is_not_ambigous(t_process *target)
{
	t_group		*groups;
	size_t		count;

	count = 0;
	if (!target)
		return (false);
	groups = g_session.groups;
	while (groups != g_session.nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next \
				&& groups->nil->next->data \
				&& !ft_strncmp(groups->nil->next->data[0], \
				target->data[0], ft_strlen(target->data[0])))
			count++;
		groups = groups->next;
	}
	return (count == 1);
}
