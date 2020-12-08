/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/12/08 14:58:35 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <string.h>
#include <path.h>

// TODO: Chdir using subshell session
int				ft_chdir(const char *path, t_exec *args)
{
	struct stat	stats;

	stat(path, &stats);
	if (S_ISDIR(stats.st_mode))
	{
		if (!(stats.st_mode & S_IRUSR))
			ft_dprintf(STDERR_FILENO, "%s: cd: %s: Permission denied\n",
				args->session->name, path);
		else if (!(args->session->flags & PIPED_CMD))
			return ((chdir(path) == -1) ? STD_ERROR : SUCCESS);
		else
			return (SUCCESS);
	}
	else
		ft_dprintf(STDERR_FILENO, "%s: %s: Not a directory\n",
			args->session->name, path);
	return (STD_ERROR);
}

// TODO: Secure
void			swap_pwds(const char *newpwd, t_exec *args)
{
	const char	*pwd;
	char		*freed;

	pwd = env_get(args->session->env, "PWD", 3);
	env_set(&args->session->env, "OLDPWD", pwd, true);
	env_set(&args->session->env, "PWD", newpwd, true);
	env_set(&args->session->env, "DIRNAME", (freed = ft_basename(newpwd)), true);
	free(freed);
}

int				go_home(t_exec *args)
{
	const char	*home_dir;

	if (!(home_dir = env_get(args->session->env, "HOME", 4)))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: HOME not set\n", \
			args->session->name, args->av[0]);
		return (STD_ERROR);
	}
	if (ft_chdir(home_dir, args))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", \
			args->session->name, args->av[0], strerror(errno));
		return (STD_ERROR);
	}
	swap_pwds(home_dir, args);
	return (SUCCESS);
}

// TODO: chdir 
int				go_to_path(char *path, t_exec *args)
{
	int			ret;

	if (!getcwd(args->session->cwd, sizeof(args->session->cwd) - 1))
	{
		ft_dprintf(STDERR_FILENO, "%s: getcwd: %s\n",
			args->session->name, strerror(errno));
		return (STD_ERROR);
	}
	if ((ret = ft_chdir(path, args)) == 0)
	{
		if (!getcwd(args->session->cwd, sizeof(args->session->cwd) - 1))
		{
			ft_dprintf(STDERR_FILENO, "%s: getcwd: %s\n",
				args->session->name, strerror(errno));
			return (STD_ERROR);
		}
		swap_pwds(args->session->cwd, args);
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
		if (path[0] == '/' && ft_chdir(path, args) == 0)
		{
			swap_pwds(path, args);
			return (SUCCESS);
		}
		else if (path[0] == '-'
		&& (oldpwd = (char*)env_get(args->session->env, "OLDPWD", 6))
		&& (oldpwd = ft_strdup(oldpwd)) && ft_chdir(oldpwd, args) == 0)
		{
			swap_pwds(oldpwd, args);
			free(oldpwd);
			return (SUCCESS);
		}
		else
			return (go_to_path(path, args));
	}
	return (STD_ERROR);
}
