/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 03:52:48 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:35:57 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

bool			is_jobspec(const char *string)
{
	return (*string == '%');
}

bool			is_string_digit(const char *string)
{
	int			i;

	i = -1;
	while (string[++i])
		if (string[i] > '9' || string[i] < '0')
			return (false);
	return (i);
}

int				matrix_height(char **matrix)
{
	char		**it;

	it = matrix;
	while (*it)
		it++;
	return (it - matrix);
}

bool			is_not_ambigous_v2(const char *niddle)
{
	t_group		*groups;
	int			count;
	int			error;

	count = -1;
	error = 0;
	groups = g_session.groups;
	while (groups != g_session.nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->data)
		{
			while (++count < \
				matrix_height((char**)groups->active_processes->data))
				if (ft_strnstr(groups->active_processes->data[count], \
					niddle, ft_strlen(niddle)))
					error++;
		}
		groups = groups->next;
	}
	return (error == 1);
}
