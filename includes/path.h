#ifndef PATH_H
# define PATH_H

# include <limits.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <stdbool.h>

# include <libft.h>

char	*path_cat(const char *a, const char *b, bool *err_alloc);
char	*path_get(const char *name, const char *path, bool *err_alloc);

#endif
