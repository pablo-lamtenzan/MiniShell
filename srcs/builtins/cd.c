/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/23 17:10:24 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <unistd.h>

int		ft_cd(int ac, char **argv, t_data *data)
{
	char	path[PATH_MAX];
	char	cwd[PATH_MAX];

	if (ac == 1)
	{
		chdir(map_get(data->env, "HOME")->value);
		map_set(&data->env, "PWD", map_get(data->env, "HOME")->value);
		return (0);
	}
	else
	{
		ft_bzero(path, PATH_MAX);
		ft_memcpy(path, argv[1], ft_strlen(argv[1]));
		if (path[0] == '/' && chdir(path) == 0)
		{
			map_set(&data->env, "PWD", path);
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
				map_set(&data->env, "PWD", cwd);
				return (0);
			}
		}
	}
	return (1);
}
