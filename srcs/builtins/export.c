/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:52 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:53:53 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <errors.h>

int	print_env(int fd, t_map *env)
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

int	ft_export(t_exec *args, t_term *t)
{
	t_map	*var;
	(void)var;
	(void)t;

	if (args->ac == 1)
	{
		//map_sort(&t->env, &map_cmp);
		//print_env(args->fds[1], t->env);
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
