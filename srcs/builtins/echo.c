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
			ft_dprintf(args->fds[FDS_STDOUT], args->ac != 1 ? "%s " : "%s",
				args->av[arg++]);
	}
	if (!opt)
		ft_dprintf(args->fds[FDS_STDOUT], "\n");
	return (SUCCESS);
}
