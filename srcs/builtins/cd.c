/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:57:11 by plamtenz          #+#    #+#             */
/*   Updated: 2020/12/12 21:59:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>
#include <string.h>
#include <path.h>

static int		swap_pwds(t_session *sess)
{
	char			*basename;
	const char		*old_pwd;
	t_exec_status	status;

	basename = NULL;
	status = SUCCESS;
	if (!(getcwd(sess->cwd, sizeof(sess->cwd) - 1)
	&& (basename = ft_basename(sess->cwd))
	&& (old_pwd = env_get(sess->env, "PWD", 3))
	&& env_set(&sess->env, "OLDPWD", old_pwd, true)
	&& env_set(&sess->env, "PWD", sess->cwd, true)
	&& env_set(&sess->env, "DIRNAME", basename, true)))
	{
		ft_dprintf(STDERR_FILENO, "%s: cd: %s\n", sess->name, strerror(errno));
		status = STD_ERROR;
	}
	free(basename);
	return (status);
}

static int		ft_chdir(t_session *sess, const char *path)
{
	struct stat	stats;

	stat(path, &stats);
	if (S_ISDIR(stats.st_mode))
	{
		if (!(stats.st_mode & S_IRUSR))
			ft_dprintf(STDERR_FILENO, "%s: cd: %s: Permission denied\n",
				sess->name, path);
		else if (chdir(path) == 0)
			return (swap_pwds(sess));
		else
			ft_dprintf(STDERR_FILENO, "%s: cd: %s\n", \
				sess->name, strerror(errno));
	}
	else
		ft_dprintf(STDERR_FILENO, "%s: %s: Not a directory\n",
			sess->name, path);
	return (STD_ERROR);
}

int				b_cd(t_exec *args)
{
	const char	*path;

	if (args->ac >= 2)
	{
		if (*(path = args->av[1]) == '-'
		&& !(path = env_get(args->session->env, "OLDPWD", 6)))
		{
			ft_dprintf(STDERR_FILENO, "%s: cd: OLDPWD not set\n",
				args->session->name);
			return (STD_ERROR);
		}
	}
	else if (!(path = env_get(args->session->env, "HOME", 4)))
	{
		ft_dprintf(STDERR_FILENO, "%s: cd: HOME not set\n",
			args->session->name);
		return (STD_ERROR);
	}
	return (ft_chdir(args->session, path));
}
