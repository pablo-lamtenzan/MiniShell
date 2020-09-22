#include <minishell.h>

int	ft_echo(int ac, char **argv)
{
	char	opt;
	short	arg;
//	char	**env;

// TODO:	env = envp;
	arg = 1;
	opt = 0;
	if (ac > 1)
	{
		if ((opt = (ft_strncmp("-n", argv[1], 3) == 0)))
		{
			ac--;
			arg++;
		}
		while (--ac)
			ac != 1 ? ft_printf("%s ", argv[arg++])
			: ft_printf("%s", argv[arg++]);
	}
	!opt ? ft_printf("\n") : 0;
	return (0);
}
