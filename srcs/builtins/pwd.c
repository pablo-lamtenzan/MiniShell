#include <minishell.h>
#include <unistd.h>

int		ft_pwd(t_builtin_args *args)
{
	char	cwd[PATH_MAX];

	(void) args;
	if (!(getcwd(cwd, sizeof(cwd))))
		return (1);
	ft_printf("%s\n", cwd);
	return (0);
}
