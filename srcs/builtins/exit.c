/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:46 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:06:05 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <term/term.h>

int					ft_atoi_exit(const char *str, bool *st)
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

int		b_exit(t_exec *args) // TODO: Callbacks in ft_term.h
{
	int i;
	bool st;

	i = g_session.st;
	st = true;
	if (args && args->ac > 1)
	{
		i = -1;
		while (args->av[1][++i])
			if (!ft_isdigit(args->av[1][i]))
				st = false;
		if (st == true && args->ac > 2)
		{
			write(STDERR_FILENO, "exit\n", 5);
			ft_dprintf(STDERR_FILENO, "%s\n", "minish: exit: too many arguments");
			return (STD_ERROR);
		}
		i = ft_atoi_exit(args->av[1], &st);
	}
	// TODO: resume_suspended_processes(&t->suspended_processes);
	term_destroy();
	//write(STDERR_FILENO, "exit\n", 5);
	// check if is 255
	if (st == false && (i = INV_EXIT_ARG))
		ft_dprintf(STDERR_FILENO, "minish: exit: %s%s\n", args->av[1],": numeric argument required");
	tputs(g_term.caps.mode.insert_end, 0, &ft_putchar);
	// debug
	ft_dprintf(STDERR_FILENO, "[DEBUG]exiting with status code %d\n", (unsigned char)i);
	handle_exit_with_active_background((unsigned char)i);
	return (STD_ERROR);
}
