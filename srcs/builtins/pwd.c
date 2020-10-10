#include <minishell.h>
#include <unistd.h>

int		ft_pwd(t_args *args, t_term *t)
{
	char	cwd[PATH_MAX];

	(void) args;
	(void) t;
	if (!(getcwd(cwd, sizeof(cwd))))
		return (1);
	ft_printf("%s\n", cwd);
	return (0);
}
