/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_separators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 10:27:06 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 21:59:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>

size_t		get_separators_nb(char *input, const char **separators)
{
	size_t	size;
	size_t	aux;
	int		i;
	int		y;

	i = -1;
	size = 0;
	while (input[++i])
	{
		y = -1;
		while (separators[++y])
		{
			if (!ft_strncmp(separators[y], &input[i], \
				aux = ft_strlen(separators[y])))
			{
				size++;
				i += (int)aux - 1;
				break ;
			}
		}
	}
	return (size + 1);
}

size_t		get_elem_size(char *input, int *i, const char **separators)
{
	int		y;
	size_t	size;

	size = 1;
	while (input[*i] && input[++(*i)])
	{
		y = -1;
		while (separators[++y])
		{
			if (!ft_strncmp(separators[y], &input[*i], \
				ft_strlen(separators[y])))
				return (size);
		}
		size++;
	}
	return (size);
}

int			copy_inter_seps(char **res, char *input, const char **separators)
{
	size_t	aux;
	int		vars[4];

	ft_bzero(&vars, sizeof(vars));
	while (input[vars[1]])
	{
		vars[0] = -1;
		while (separators[++vars[0]])
		{
			if (!ft_strncmp(&input[vars[1]], separators[vars[0]], \
				aux = ft_strlen(separators[vars[0]])))
			{
				vars[1] += (int)aux;
				vars[0] = -1;
			}
		}
		vars[3] = vars[1];
		if ((aux = get_elem_size(input, &vars[1], separators)))
		{
			if (!(res[vars[2]++] = ft_strndup(&input[vars[3]], aux)))
				return (false);
		}
	}
	return (true);
}

char		**split_separators(char *input)
{
	static const char	*seps[4] = {"||", "&&", ";", NULL};
	char				**res;

	if ((res = ft_calloc(get_separators_nb(input, seps) + 1, sizeof(char*)))
	&& !(copy_inter_seps(res, input, seps)))
	{
		strs_unload(res);
		res = NULL;
	}
	return (res);
}
