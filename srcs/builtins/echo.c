#include <minishell.h>

int	ft_echo(int ac, char* *av)
{
	char	opt;
	short	arg;
//	char	**env;

// TODO:	env = envp;
	arg = 1;
	opt = 0;
	if (ac > 1)
	{
		if ((opt = (ft_strncmp("-n", av[1], 3) == 0)))
		{
			ac--;
			arg++;
		}
		while (--ac)
			ac != 1 ? ft_printf("%s ", av[arg++])
			: ft_printf("%s", av[arg++]);
	}
	!opt ? ft_printf("\n") : 0;
	return (0);
}
