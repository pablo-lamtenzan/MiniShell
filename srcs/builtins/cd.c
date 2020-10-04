/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/04 20:19:29 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: Check allocation errors on map_set

int		ft_cd(int ac, char* *av, t_term *term)
{
	char	path[PATH_MAX];
	char	cwd[PATH_MAX];

	if (ac == 1)
	{
		chdir(map_get(term->env, "HOME")->value);
		map_set(&term->env, "PWD", map_get(term->env, "HOME")->value);
		return (0);
	}
	else
	{
		ft_bzero(path, PATH_MAX);
		ft_memcpy(path, av[1], ft_strlen(av[1]));
		if (path[0] == '/' && chdir(path) == 0)
		{
			map_set(&term->env, "PWD", path);
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
				map_set(&term->env, "PWD", cwd);
				return (0);
			}
		}
	}
	return (1);
}
