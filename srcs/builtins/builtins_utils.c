/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 18:03:18 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 22:30:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

int				parse_flags(int ac, char*const *av, const char *pattern,
		int *nb_flags)
{
	int			cont;
	int			flags;
	int 		i;
	int			prev;
	*nb_flags = 0;

	cont = -1;
	flags = 0;
	while (++cont < ac - 1)
	{
		i = -1;
		while (av[cont][++i])
		{
			if (av[cont][i] == '-')
			{
				if (i == 0)
					continue ;
				else
					return (-1);
			}
			prev = flags;
			if ((flags |= (1 << ft_strpos(pattern, av[cont][i]))) < 0)
				return (!*nb_flags ? -1 : -prev);
		}
		(*nb_flags)++;
	}
	return (flags);
}

bool			ignore_pid(int ac, char*const *av)
{
	(void)ac;
	if (is_string_digit(av[1]))
		return (false);
	return (true);
}

const char		*is_in_history(t_process* target)
{
	if (!g_session->hist)
		return (" ");
	if (background_find(target, "PID", g_session->hist->group) \
			&& is_leader(target))
		return ("+");
	else if (g_session->hist->next \
			&& background_find(target, "PID", g_session->hist->next->group) \
			&& is_leader(target))
		return ("-");
	else
		return (" ");
}