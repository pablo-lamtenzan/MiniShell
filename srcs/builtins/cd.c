/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/12/02 11:49:15 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <string.h>
// TODO: Check allocation errors on map_set


int	ft_chdir(const char *path)
{
	struct stat stats;

	stat(path, &stats);
	if (S_ISDIR(stats.st_mode) && !(stats.st_mode & S_IRUSR))
		return (42);
	else if (S_ISDIR(stats.st_mode) && g_session.flags & PIPED_CMD)
		return (0);
	else
		return (chdir(path));
}

void	swap_pwds(const char *newpwd)
{
	const char*	pwd;

	pwd = env_get(g_session.env, "PWD", 3);
	env_set(&g_session.env, "OLDPWD", pwd, true);
	env_set(&g_session.env, "PWD", newpwd, true);
}

int	b_cd(t_exec *args)
{
	const char	*home_dir;
	const char	*oldpwd;
	char		path[PATH_MAX];
	char		cwd[PATH_MAX];
	int			ret;

	if (!(oldpwd = env_get(g_session.env, "OLDPWD", 6)))
		return (STD_ERROR);
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
		swap_pwds(home_dir);
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
		else if (path[0] == '-' && chdir(oldpwd) == 0)
		{
			swap_pwds(oldpwd);
			return (SUCCESS);
		}
		else
		{
			if (!(getcwd(cwd, sizeof(cwd))))
				return (1);
			ft_strlcat(cwd, "/", ft_strlen(cwd) + 2);
			ft_strlcat(cwd, path, ft_strlen(cwd) + 2 + ft_strlen(path));
			if ((ret = ft_chdir(cwd)) == 0)
			{
				getcwd(cwd, sizeof(cwd));
				swap_pwds(cwd);
				return (SUCCESS);
			}
			else if (ret == 42)
				ft_dprintf(STDERR_FILENO, "%s: cd: %s: Permission denied\n", \
					g_session.name, args->av[1]);
			else
				ft_dprintf(STDERR_FILENO, "%s: %s: Not a directory\n", \
					g_session.name, args->av[1]);
		}
	}
	return (STD_ERROR);
}
