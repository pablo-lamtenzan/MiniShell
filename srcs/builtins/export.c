/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:52 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 01:40:47 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int				print_env(int fd, t_map *env)
{
	if (env)
	{
		if (env->value && *env->value)
			ft_printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			ft_printf("declare -x %s\n", env->key);
		print_env(fd, env->next);
	}
	return (SUCCESS);
}

int	b_export(t_exec *args)
{
	if (args->ac == 1)
	{
		//map_sort(&t->env, &map_cmp);
		//print_env(args->fds[FDS_STDOUT], t->env);
		;
	}
	else
	{
		while (args->ac-- > 1)
		{
			/* if (!key_check(av[ac])) // TODO: Key checks on export builtin
			{
				ft_printf("%s: %s `%s' : not a valid identifier\n",
					t->name, av[0], av[ac]);
				return (1);
			} */
			/*
			if ((var = map_get(t->env, args->av[args->ac])))
			{
				if (!map_set(&t->env, var->key, var->value))
					return (1);
			}
			else if (!map_set(&t->env, args->av[args->ac], ""))
				return (1);
			*/
		;
		}
	}
	return (SUCCESS);
}
