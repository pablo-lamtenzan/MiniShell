/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parse_name.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:49:49 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 22:00:46 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

// add here get_process by name when it will be finished
t_process**		get_process_by_name(t_group* groups, const char* av)
{
	// %name
	// %?name
	ft_dprintf(2, "[NAME][name: %s]\n", av);
	const int	search_mode = get_search_mode(av); // lexer
	int			match;
	t_process**	ret;
	int			count;

	if (search_mode == 0)
		return (NULL);
	match = 0;
	ret = NULL;

	if (0)
		ft_dprintf(2, "%p ------------ %p\n", g_session->groups, groups);
	while (groups != g_session->nil)
	{
		while (groups->active_processes != groups->nil)
		{
			count = -1;
			if (search_mode == 1) // %name
			{
				if (0)
					ft_dprintf(2, "TEST  %s --- %s \n", groups->active_processes->data[0], av);
				if (!ft_strncmp(groups->active_processes->data[0], av, ft_strlen(groups->active_processes->data[0])) && (++match))
				{
					if (0)
						ft_dprintf(2, "[DATA][FOUND: %p]\n", groups->active_processes);
					if (is_leader(groups->active_processes) && is_not_ambigous(groups->active_processes))
						return(&groups->active_processes);
				}
			}
			else if (search_mode == 2) // %?name
			{
				// TO DO: debug this
				while (++count < matrix_height((char**)groups->active_processes->data)) // change matrix height later
					if (ft_strnstr(groups->active_processes->data[count], &av[1], ft_strlen(&av[1]) && (++match)))
					{
						if (0)
							ft_dprintf(2, "[DATA][FOUND: %p]\n", groups->active_processes);
						if (is_leader(groups->active_processes) && is_not_ambigous_v2(&av[1]))
						return(&groups->active_processes);
					}
			}
			groups->active_processes = groups->active_processes->next;
		}
		groups = groups->next;
	}
	ft_dprintf(2, "[DATA][NOT FOUND OR AMBIGOUS!!!!]\n");
	return (NULL);
}

bool			is_not_ambigous(t_process* target)
{
	t_group*	groups;
	size_t		count;

	count = 0;
	if (!target)
		return (false);
	groups = g_session->groups;
	while (groups != g_session->nil && groups->nil && groups->nil->next)
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

bool			is_not_ambigous_v2(const char* niddle)
{
	t_group*	groups;
	int			count;
	int			error;

	count = -1;
	error = 0;
	groups = g_session->groups;
	while (groups != g_session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->data)
		{
			// TO DO: CHECK THE DOUBLE NIDLE IN SAME WORD
			while (++count < matrix_height((char**)groups->active_processes->data))
				if (ft_strnstr(groups->active_processes->data[count], niddle, ft_strlen(niddle)))
					error++;
		}
		groups = groups->next;
	}
	return (error == 1);
}
