#include <path.h>

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

/* char	*path_get(const char *name, const char *path)
{
	char		**paths;
	char		*join;
	struct stat	s;
	int			i;

	if (!name || !*name || !(paths = ft_split(path, ':')))
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if (!(join = path_cat(paths[i++], name)))
			return (strs_unload(paths));
		if (stat(join, &s) == 0 && s.st_mode & S_IXUSR)
		{
			strs_unload(paths);
			return (join);
		}
		free(join);
	}
	strs_unload(paths);
	return (NULL);
}
 */

// TODO: IDK why there is a new version

char	*path_get(const char *name, const char *path)
{
	char		**paths;
	char		*absolute;
	struct stat	s;
	int			i;

	absolute = NULL;
	if (name && *name)
	{
		if (*name == '/' && stat(name, &s) == 0 && s.st_mode & S_IXUSR)
			absolute = ft_strdup(name);
		else if ((paths = ft_split(path, ':')))
		{
			i = 0;
			while (paths[i]
			&& (absolute = path_cat(paths[i++], name))
			&& !(stat(absolute, &s) == 0 && s.st_mode & S_IXUSR))
				free(absolute);
			if (!paths[i])
				absolute = NULL;
			strs_unload(paths);
		}
	}
	ft_dprintf(2, "[PATH][ABSOLUTE] '%s'\n", absolute);
	return (absolute);
}
