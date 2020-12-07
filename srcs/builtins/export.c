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
	if (!env)
		return ;
	if (env->exported)
		ft_dprintf(fd, "declare -x %s\n", env->key);
	print_env(fd, env->next);
}

static char	*trim_name(const char *s)
{
	char	*trim;
	size_t	size;

	size = 0;
	while (s[size] && s[size] != '=')
		size++;
	if (!(trim = malloc(sizeof(char) * size + 1)))
		return (NULL);
	trim[size] = 0;
	ft_memcpy(trim, s, size);
	return (trim);
}

int			b_export(t_exec *args)
{
	t_env	*first;
	char	*freed;
	bool	assign;
	int		ret;
	int		i;

	ret = SUCCESS;
	i = -1;
	if (args->ac == 1)
	{
		print_env(args->fds[FDS_STDOUT], *args->env);
		return (ret);
	}
	while (++i < args->ac)
	{
		first = *args->env;
		assign = (bool)ft_strchr(args->av[i], '=');
		freed = NULL;
		while (!assign && *args->env)
		{
			if ((freed = trim_name(args->av[i])))
				return (STD_ERROR);
			if (!ft_strncmp(freed, (*args->env)->key, ft_strlen(freed)))
				(*args->env)->exported = true;
			// problem doesn't print the exported true
			*args->env = (*args->env)->next;
			free(freed);
			ret = SUCCESS;
		}
		*args->env = first;
		if (!assign && !freed)
			ret = STD_ERROR;
		if (assign)
		{
			if (!(freed = trim_name(args->av[i])))
				return (STD_ERROR);
			// Problem export the var has no value
			ft_dprintf(2, "size: %lu\n", ft_strlen(freed) + 1);
			env_set(args->env, freed, (char*)&args->av[i] + ft_strlen(freed) + 1, true);
			ret = SUCCESS;
		}
	}
	return (ret);
}
