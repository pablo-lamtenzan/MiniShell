#include <path.h>
#include <errors.h>
#include <job_control/session.h>
#include <unistd.h>

char	*path_cat(const char *a, const char *b)
{
	const size_t	len = ft_strlen(a) + ft_strlen(b) + 1;
	char			*cat;
	char			*c;

	if (len > PATH_MAX || !(cat = ft_calloc(len + 1, sizeof(*cat))))
		return (NULL);
	c = cat;
	while (*a)
		*c++ = *a++;
	*c++ = '/';
	while (*b)
		*c++ = *b++;
	return (cat);
}

static int		path_error(const char *name)
{
	struct stat	s;
	int			res;
	const int	st = stat(name, &s);

	res = SUCCESS;
	if (!ft_strncmp(name, "./", 2))
	{
		if (st == 0 && S_ISDIR(s.st_mode))
			res = ft_dprintf(STDERR_FILENO, \
				"%s: %s: Is a directory\n", g_session.name, name);
		else if (st == 0 && !(s.st_mode & S_IXUSR))
			res = ft_dprintf(STDERR_FILENO, \
				"%s: %s: Permission denied\n", g_session.name, name);
		else if (st)
			res = ft_dprintf(STDERR_FILENO, \
				"%s: %s: No such file or directory\n", g_session.name, name);
	}
	else
	{
		if (st == 0 && ft_strchr(name, '/') && S_ISDIR(s.st_mode))
			res = ft_dprintf(STDERR_FILENO, \
				"%s: %s: Is a directory\n", g_session.name, name);
		else if (st && ft_strchr(name, '/'))
			res = ft_dprintf(STDERR_FILENO, \
				"%s: %s: No such file or directory\n", g_session.name, name);
	}
	return (res);
}

char	*path_get(const char *name, const char *path, bool *err_alloc)
{
	char		**paths;
	char		*absolute;
	struct stat	s;
	size_t		i;

	absolute = NULL;
	if (name && *name && path_error(name) == 0)
	{
		if (*name == '/' || *name == '.')
		{
			if (stat(name, &s) == 0 && s.st_mode & S_IXUSR)
				absolute = ft_strdup(name);
		}
		else if ((paths = ft_split(path, ':')))
		{
			i = 0;
			while (paths[i]
			&& (absolute = path_cat(paths[i], name))
			&& !(stat(absolute, &s) == 0 && s.st_mode & S_IXUSR))
			{
				i++;
				free(absolute);
			}
			if (!absolute && (*err_alloc = true))
				return (NULL);
			if (!paths[i])
			{
				absolute = NULL;
				ft_dprintf(STDERR_FILENO, "%s: %s: command not found\n",
						g_session.name, name);
			}
			strs_unload(paths);
		}
	}
	return (absolute);
}
