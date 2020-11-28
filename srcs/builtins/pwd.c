/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:55 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 01:40:58 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

// TODO: Redirections for STDERR_FILENO

int		ft_pwd(t_exec *args)
{
	char			cwd[PATH_MAX];

	if (args->ac > 1)
		ft_dprintf(STDERR_FILENO, "%s\n", "pwd: too many arguments");
	else
	{
		if (!(getcwd(cwd, sizeof(cwd))))
			return (STD_ERROR);
		ft_dprintf(args->fds[FDS_STDOUT], "%s\n", cwd);
	}
	return (SUCCESS);
}
