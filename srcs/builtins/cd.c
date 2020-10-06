/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 01:44:28 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: Check allocation errors on map_set

int		ft_cd(t_builtin_args *args)
{
	char	path[PATH_MAX];
	char	cwd[PATH_MAX];

	if (args->ac == 1)
	{
		chdir(map_get(args->t->env, "HOME")->value);
		map_set(&args->t->env, "PWD", map_get(args->t->env, "HOME")->value);
		return (0);
	}
	else
	{
		ft_bzero(path, PATH_MAX);
		ft_memcpy(path, args->av[1], ft_strlen(args->av[1]));
		if (path[0] == '/' && chdir(path) == 0)
		{
			map_set(&args->t->env, "PWD", path);
			return (0);
		}
		else
		{
			if (!(getcwd(cwd, sizeof(cwd))))
				return (1);
			ft_strlcat(cwd, "/", ft_strlen(cwd) + 2);
			ft_strlcat(cwd, path, ft_strlen(cwd) + 2 + ft_strlen(path));
			if (chdir(cwd) == 0)
			{
				getcwd(cwd, sizeof(cwd));
				map_set(&args->t->env, "PWD", cwd);
				return (0);
			}
		}
	}
	return (1);
}
