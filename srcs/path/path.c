#include <path.h>
#include <job_control.h>
#include <unistd.h>

static char	*path_cat(const char *a, const char *b)
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

char	*path_get(const char *name, const char *path)
{
	char		**paths;
	char		*absolute;
	struct stat	s;
	size_t		i;

	absolute = NULL;
	if (name && *name)
	{
		if (*name == '/' || *name == '.')
		{
			if (stat(name, &s) == 0 && S_ISDIR(s.st_mode))
			{
				ft_dprintf(STDERR_FILENO, "%s: %s: Is a directory\n", g_session.name, name);
				return (NULL);
			}
			if (s.st_mode & S_IXUSR)
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
			if (!paths[i])
				absolute = NULL;
			strs_unload(paths);
		}
	}
	return (absolute);
}
