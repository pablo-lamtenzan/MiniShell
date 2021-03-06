/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:46 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 21:59:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>
#include <term/term.h>
#include <job_control/background.h>

int		ft_atoi_exit(const char *str, bool *st)
{
	char			factor;
	unsigned long	max;
	unsigned long	i;

	while (ft_isspace(*str))
		str++;
	factor = (*str == '-') ? -1 : 1;
	str += (*str == '-' || *str == '+');
	max = (factor < 0) ? 0x8000000000000000UL : 0x7FFFFFFFFFFFFFFFL;
	i = 0;
	while (ft_isdigit(*str) && i < max)
		i = i * 10 + (*str++ - '0');
	if (i >= max)
		*st = false;
	return (!st ? 255 : i * factor);
}

int		b_exit(t_exec *args)
{
	int		i;
	bool	st;

	i = args->session->st;
	st = true;
	if (args && args->ac > 1)
	{
		i = -1;
		while (args->av[1][++i])
			if (!ft_isdigit(args->av[1][i]))
				st = false;
		if (st == true && args->ac > 2)
		{
			ft_dprintf(STDERR_FILENO, "%s\n", \
				"minish: exit: too many arguments");
			return (STD_ERROR);
		}
		i = ft_atoi_exit(args->av[1], &st);
	}
	if (st == false && (i = INV_EXIT_ARG))
		ft_dprintf(STDERR_FILENO, "minish: exit: %s%s\n", \
			args->av[1], ": numeric argument required");
	handle_exit_with_active_background((unsigned char)i, args->session);
	return ((unsigned char)i);
}
