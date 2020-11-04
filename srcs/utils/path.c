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

char	*path_get(const char *name, char *path)
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
