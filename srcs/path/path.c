/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 05:46:07 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/13 01:15:37 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <path.h>
#include <errors.h>
#include <job_control/session.h>
#include <unistd.h>



// TODO: Match PATH_MAX check with redirections
static bool		is_filepath(const char *name, bool print_err)
{
	size_t	path_len;
	size_t	name_len;
	const char	*next;
	const char	*last;
	bool	valid;

	path_len = 0;
	last = delim_skip(name);
	while ((next = ft_strchr(last, PATH_DELIM))
	&& (name_len = next - last) <= NAME_MAX)
	{
		path_len += name_len + 1;
		last = delim_skip(next + 1);
	}
	if (!(valid = !(path_len > PATH_MAX || next || ft_strlen(last) > NAME_MAX))
	&& print_err)
		ft_dprintf(STDERR_FILENO, "%s: %s: Filename too long\n",
			g_session.name, name);
	return (valid);
}

static bool		is_executable(const char *name, bool print_err)
{
	struct stat	st;
	const bool	error = stat(name, &st);
	bool		valid;

	valid = false;
	if (!print_err)
		valid = !error && !S_ISDIR(st.st_mode) && (st.st_mode & S_IXUSR);
	else if (error)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n",
			g_session.name, name, strerror(errno));
	else
	{
		if (S_ISDIR(st.st_mode))
			ft_dprintf(STDERR_FILENO, "%s: %s: Is a directory\n",
				g_session.name, name);
		else if (!(st.st_mode & S_IXUSR))
			ft_dprintf(STDERR_FILENO, "%s: %s: Permission denied\n",
				g_session.name, name);
		else
			valid = true;
	}
	return (valid);
}

bool	path_get(char **dest, const char *name, const char *path)
{
	static char	real[PATH_MAX + 1];
	char		**paths;
	size_t		i;

	i = 0;
	*dest = NULL;
	if (name && *name && is_filepath(name, true))
	{
		if (*name == '/' || *name == '.' || !path || !*path)
		{
			if (is_executable(name, true))
				ft_strlcpy((*dest = real), name, sizeof(real));
			return (true);
		}
		else if ((paths = ft_split(path, ':')))
		{
			while (paths[i] && (!path_cat(real, paths[i], name)
			|| !(is_filepath(real, false) && is_executable(real, false))))
				i++;
			if (paths[i])
				*dest = real;
			strs_unload(paths);
		}
		else
			return (false);
	}
	if (!*dest)
		ft_dprintf(STDERR_FILENO, "%s: %s: command not found\n",
			g_session.name, name);
	return (true);
}
