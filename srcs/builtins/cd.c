/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/12/02 10:22:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <string.h>
// TODO: Check allocation errors on map_set


int	ft_chdir(const char *path)
{
	struct stat stats;

	stat(path, &stats);
	if (S_ISDIR(stats.st_mode) && g_session.flags & PIPED_CMD)
		return (0);
	else
		return (chdir(path));
}

int	b_cd(t_exec *args)
{
	const char	*home_dir;
	char		path[PATH_MAX];
	char		cwd[PATH_MAX];

	if (args->ac == 1)
	{
		if (!(home_dir = env_get(g_session.env, "HOME", 4)))
		{
			ft_dprintf(STDERR_FILENO, "%s: %s: HOME not set\n", g_session.name, args->av[0]);
			return (STD_ERROR);
		}
		if (ft_chdir(home_dir))
		{
			ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", g_session.name, args->av[0], strerror(errno));
			return (STD_ERROR);
		}
		env_set(&g_session.env, "PWD", home_dir, true);
		return (SUCCESS);
	}
	else
	{
		ft_bzero(path, PATH_MAX);
		ft_memcpy(path, args->av[1], ft_strlen(args->av[1]));
		if (path[0] == '/' && ft_chdir(path) == 0)
		{
			env_set(&g_session.env, "PWD", path, true);
			return (SUCCESS);
		}
		else
		{
			if (!(getcwd(cwd, sizeof(cwd))))
				return (1);
			ft_strlcat(cwd, "/", ft_strlen(cwd) + 2);
			ft_strlcat(cwd, path, ft_strlen(cwd) + 2 + ft_strlen(path));
			if (ft_chdir(cwd) == 0)
			{
				getcwd(cwd, sizeof(cwd));
				env_set(&g_session.env, "PWD", cwd, true);
				return (SUCCESS);
			}
			else
				ft_dprintf(STDERR_FILENO, "cd: no such file or directory: %s\n", args->av[1]);
		}
	}
	return (STD_ERROR);
}
