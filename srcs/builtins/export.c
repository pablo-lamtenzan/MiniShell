/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:52 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 13:18:51 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static void	print_env(int fd, t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			if (env->key[env->key_length] == ENV_OP_ASSIGN)
				ft_dprintf(fd, "declare -x %.*s=\"%s\"\n",
					(int)env->key_length, env->key, env->key + env->key_length + 1);
			else
				ft_dprintf(fd, "declare -x %s\n", env->key);
		}
		env = env->next;
	}
}

// TODO: Maybe print errno on allocation error (STD_ERROR)
int			b_export(t_exec *args)
{
	int		ret;
	int		i;
	size_t	key_len;
	char	assign_st;

	ret = SUCCESS;
	i = 0;
	if (args->ac == 1)
	{
		print_env(args->fds[FDS_STDOUT], *args->env);
		return (ret);
	}
	while (++i < args->ac)
	{
		if ((key_len = env_key_len(args->av[i], true)))
		{
			if ((assign_st = env_assign(args->env, args->av[i], true, true)) == 0)
				env_set(args->env, args->av[i], NULL, true);
			else if (assign_st == -1)
				return (STD_ERROR);
		}
		else
		{
			ft_dprintf(STDERR_FILENO,
				"%s: export: `%s': not a valid identifier\n",
					g_session.name, args->av[i]);
		}
	}
	return (ret);
}
