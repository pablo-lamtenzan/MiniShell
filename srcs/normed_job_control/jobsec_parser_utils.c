/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobsec_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:51:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 01:52:50 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

size_t			get_search_mode(const char* av)
{
	ft_dprintf(2, "AV : %s\n", av);
	if (av[0] && av[0] != '?')
		return (1);
	if (av[0] == '?' && av[1])
		return (2);
	else
		return (0);
}

size_t			get_history_index(const char* key)
{
	if (key[0] == '+' || key[0] == '%' || key[0] == '\0')
		return (1);
	if (key[0] == '-')
		return (2);
	else
		return (0);
}

bool			is_jobspec(const char* string)
{
	return (*string == '%');
}

bool			is_string_digit(const char* string)
{
	int			i;

	i = -1;
	while (string[++i])
		if (string[i] > '9' || string[i] < '0')
			return (false);
	return (i);
}

int			matrix_height(char **matrix)
{
	char** it;

	it = matrix;
	while (*it)
		it++;
	return (it - matrix);
}
