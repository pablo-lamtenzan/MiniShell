/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 08:19:56 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int		ft_pwd(t_exec *args, t_term *t)
{
	char	cwd[PATH_MAX];

	(void) t;

	if (args->ac > 1)
		ft_dprintf(2, "%s\n", "pwd: too many arguments");
	else
	{
		if (!(getcwd(cwd, sizeof(cwd))))
			return (1);
		ft_dprintf(args->fds[1], "%s\n", cwd);
	}
	return (0);
}
