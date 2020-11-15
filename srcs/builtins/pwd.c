/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:59:05 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

t_exec_status		ft_pwd(t_exec *args, t_term *t)
{
	char			cwd[PATH_MAX];

	(void) t;

	if (args->ac > 1)
		ft_dprintf(STDERR_FILENO, "%s\n", "pwd: too many arguments");
	else
	{
		if (!(getcwd(cwd, sizeof(cwd))))
			return (STD_ERROR);
		ft_dprintf(args->fds[1], "%s\n", cwd);
	}
	return (SUCCESS);
}
