/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/12/07 10:33:04 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <string.h>
#include <path.h>

// TODO: Chdir using subshell session
int				ft_chdir(const char *path)
{
	struct stat	stats;

	stat(path, &stats);
	if (S_ISDIR(stats.st_mode))
	{
		if (!(stats.st_mode & S_IRUSR))
			ft_dprintf(STDERR_FILENO, "%s: cd: %s: Permission denied\n",
				g_session.name, path);
		else if (!(g_session.flags & PIPED_CMD))
			return ((chdir(path) == -1) ? STD_ERROR : SUCCESS);
		else
			return (SUCCESS);
	}
	else
		ft_dprintf(STDERR_FILENO, "%s: %s: Not a directory\n",
			g_session.name, path);
	return (STD_ERROR);
}

// TODO: Secure
void			swap_pwds(const char *newpwd)
{
	const char	*pwd;
	char		*freed;

	pwd = env_get(g_session.env, "PWD", 3);
	env_set(&g_session.env, "OLDPWD", pwd, true);
	env_set(&g_session.env, "PWD", newpwd, true);
	env_set(&g_session.env, "DIRNAME", (freed = ft_basename(newpwd)), true);
	free(freed);
}

int				go_home(t_exec *args)
{
	const char	*home_dir;

	if (!(home_dir = env_get(g_session.env, "HOME", 4)))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: HOME not set\n", \
			g_session.name, args->av[0]);
		return (STD_ERROR);
	}
	if (ft_chdir(home_dir))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
			g_session.name, args->av[0], strerror(errno));
		return (STD_ERROR);
	}
	swap_pwds(home_dir);
	return (SUCCESS);
}

// TODO: chdir 
int				go_to_path(char *path)
{
	int			ret;

	if (!getcwd(g_session.cwd, sizeof(g_session.cwd) - 1))
	{
		ft_dprintf(STDERR_FILENO, "%s: getcwd: %s\n",
			g_session.name, strerror(errno));
		return (STD_ERROR);
	}
	if ((ret = ft_chdir(path)) == 0)
	{
		if (!getcwd(g_session.cwd, sizeof(g_session.cwd) - 1))
		{
			ft_dprintf(STDERR_FILENO, "%s: getcwd: %s\n",
				g_session.name, strerror(errno));
			return (STD_ERROR);
		}
		swap_pwds(g_session.cwd);
		return (SUCCESS);
	}
	return (ret);
}

int	b_cd(t_exec *args)
{
	char		*oldpwd;
	char		path[PATH_MAX];

	if (args->ac == 1)
		return (go_home(args));
	else
	{
		ft_bzero(path, sizeof(path));
		//ft_memcpy(path, args->av[1], ft_strlen(args->av[1]));
		ft_strlcpy(path, args->av[1], sizeof(path));
		if (path[0] == '/' && ft_chdir(path) == 0)
		{
			swap_pwds(path);
			return (SUCCESS);
		}
		else if (path[0] == '-'
		&& (oldpwd = (char*)env_get(g_session.env, "OLDPWD", 6))
		&& (oldpwd = ft_strdup(oldpwd)) && ft_chdir(oldpwd) == 0)
		{
			swap_pwds(oldpwd);
			free(oldpwd);
			return (SUCCESS);
		}
		else
			return (go_to_path(path));
	}
	return (STD_ERROR);
}
