#include <execution.h>

int		ft_pwd(t_exec *args, t_term *t)
{
	char	cwd[PATH_MAX];

	(void) args;
	(void) t;
	if (!(getcwd(cwd, sizeof(cwd))))
		return (1);
	ft_dprintf(args->fds[1], "%s\n", cwd);
	return (0);
}
