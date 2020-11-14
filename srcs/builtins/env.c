/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:20:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:49:53 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <errors.h>

void	env_print(int fd, t_map *env)
{
	if (env)
	{
		env_print(fd, env->next);
		ft_dprintf(fd, "%s=%s\n", env->key, env->value);
	}
}

int		ft_env(t_exec *args, t_term *t)
{
	t_env* curr;

	curr = t->env;
	(void) args;
	while (curr)
	{
		ft_putstr_fd(curr->key, args->fds[1]);
		write(args->fds[1], "\n", 1);
		curr = curr->next;
	}
	return (SUCCESS);
}
