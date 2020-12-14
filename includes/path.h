#ifndef PATH_H
# define PATH_H

# include <limits.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <stdbool.h>

# include <libft.h>

# include <string.h>
# include <errno.h>

# define PATH_DELIM	'/'


char		*path_cat(char dest[PATH_MAX + 1], const char *a, const char *b);
const char	*delim_skip(const char *path);

bool		path_get(char **dest, const char *name, const char *path);

#endif
