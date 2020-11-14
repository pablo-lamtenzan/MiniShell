/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:46 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 21:17:00 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int	ft_atoi_exit(const char *str, bool *st)
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

int		ft_exit(t_exec *args, t_term *t) // TODO: Callbacks in ft_term.h
{
	int i;
	bool st;

	i = t->st;
	st = true;
	if (args && args->ac > 1)
	{
		i = -1;
		while (args->av[1][++i])
			if (!ft_isdigit(args->av[1][i]))
				st = false;
		if (st == true && args->ac > 2)
		{
			write(2, "exit\n", 5);
			ft_dprintf(2, "%s\n", "minish: exit: too many arguments");
			return (EXIT_FAILURE);
		}
		i = ft_atoi_exit(args->av[1], &st);
	}
	term_destroy(t);
	write(2, "exit\n", 5);
	if (st == false && (i = 255))
		ft_dprintf(2, "minish: exit: %s%s\n", args->av[1],": numeric argument required");
	tputs(t->caps.insert_end, 0, &ft_putchar);
	// debug
	ft_dprintf(2, "exiting with status code %d", (unsigned char)i);
	exit((unsigned char)i);
	return ((unsigned char)i);
}
