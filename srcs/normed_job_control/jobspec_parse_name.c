/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parse_name.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:49:49 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 01:54:02 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

// add here get_process by name when it will be finished

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
