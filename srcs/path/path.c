#include <path.h>
#include <job_control.h>
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

# define PTH_EPOINT		1
# define PTH_ESLASH		2
# define PTH_EMANPT		4
# define PTH_EUNKNOWN	8

// TO DO: ?? Don't know if continue with that: theres somes execptions and we handle clean now (but not all)
static int	get_code(char *name, size_t	*skip)
{
	if (name[0] == '.')
	{
		*skip = 1;
		if (name[1] && name[1] == '/')
		{
			*skip = 2;
			return (PTH_EPOINT | PTH_ESLASH);
		}
		if (!name[1])
			return (PTH_EMANPT);
		return (PTH_EPOINT);
	}
	else if (name[0] == '/')
	{
		*skip = 0;
		return (PTH_ESLASH);
	}
	return (PTH_EUNKNOWN);
}

static int	exec_path_error(char *name)
{
	struct stat	s;
	size_t		skip;
	const int 	st = get_code(name, &skip);
	// TO DO: rmv this printf
	ft_dprintf(2, "skip: %lu\n", skip);
	const int	ret = stat(name + skip, &s);
	int			res;

	res = SUCCESS;
	if (st & PTH_EMANPT)
		res = ft_dprintf(STDERR_FILENO, "%s: %s: filename argument required\n%s: usege: %s filename [arguments]\n", g_session.name, name, name, name);
	else if (!ret && S_ISDIR(s.st_mode))
		res = ft_dprintf(STDERR_FILENO, "%s: %s: Is a directory\n", g_session.name, name);
	else if (ret && st & PTH_ESLASH)
		res = ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", g_session.name, name);
	else if (ret && st & PTH_EPOINT)
		res = ft_dprintf(STDERR_FILENO, "%s: %s command not found\n", g_session.name, name);
	else if (st & PTH_EUNKNOWN)
		res = 1;
	return (res);
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
			if (exec_path_error((char*)name) == 0 && s.st_mode & S_IXUSR)
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
