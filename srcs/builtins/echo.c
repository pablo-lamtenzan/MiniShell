#include <execution.h>

int	ft_echo(t_exec *args, t_term *t)
{
	char	opt;
	short	arg;

	(void) t;
	arg = 1;
	opt = 0;
	if (args->ac > 1)
	{
		if ((opt = (ft_strncmp("-n", args->av[1], 3) == 0)))
		{
			args->ac--;
			arg++;
		}
		while (--args->ac)
			args->ac != 1 ? ft_dprintf(args->fds[1], "%s ", args->av[arg++])
			: ft_dprintf(args->fds[1], "%s", args->av[arg++]);
	}
	!opt ? ft_dprintf(args->fds[1], "\n") : 0;
	return (0);
}
