/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:20:10 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:49:07 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int	ft_echo(t_exec *args, t_term *t)
{
	char		opt;
	short		arg;

	(void) t;
	arg = 1;
	opt = 0;
	if (args->ac > 1)
	{
		if ((opt = (ft_strncmp("-n", args->av[1], 3) == 0)))
		{
			args->ac--;
			arg++;
		}
		while (--args->ac)
			args->ac != 1 ? ft_dprintf(args->fds[1], "%s ", args->av[arg++])
			: ft_dprintf(args->fds[1], "%s", args->av[arg++]);
	}
	!opt ? ft_dprintf(args->fds[1], "\n") : 0;
	return (SUCCESS);
}
