#include <minishell.h>

int		ft_pwd(void)
{
	char cwd[PATH_MAX];

	if (!(getcwd(cwd, sizeof(cwd))))
		return (1);
	ft_printf("%s\n", cwd);
	return (0);
}