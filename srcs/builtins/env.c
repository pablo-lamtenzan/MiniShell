/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:20:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 01:23:09 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

void	env_print(int fd, t_map *env)
{
	if (env)
	{
		env_print(fd, env->next);
		ft_dprintf(fd, "%s=%s\n", env->key, env->value);
	}
}

int		ft_env(t_exec *args)
{
	t_env* 		curr;

	curr = g_session->env;
	(void) args;
	while (curr)
	{
		ft_putstr_fd(curr->key, args->fds[FDS_STDOUT]);
		write(args->fds[FDS_STDOUT], "\n", 1);
		curr = curr->next;
	}
	return (SUCCESS);
}
