#include <execution.h>

int		ft_exit(t_exec *args, t_term *t) // TODO: Callbacks in ft_term.h
{
	int i;
	bool st;

	i = t->st;
	st = true;
	if (args && args->ac > 1)
	{
		i = -1;
		while (args->av[1][++i])
			if (!ft_isdigit(args->av[1][i]))
				st = false;
		if (st == true && args->ac > 2)
		{
			write(2, "exit\n", 5);
			ft_dprintf(2, "%s\n", "minish: exit: too many arguments");
			return (EXIT_FAILURE);
		}
		i = ft_atoi(args->av[1]);
	}
	term_destroy(t);
	write(2, "exit\n", 5);
	if (st == false)
		ft_dprintf(2, "minish: exit: %s%s\n", args->av[1],": numeric argument required");
	tputs(t->caps.insert_end, 0, &ft_putchar);
	// debug
	ft_dprintf(2, "exiting with status code %d", i > 255 ? 255 : (unsigned char)i);
	exit(i > 255 ? 255 : (unsigned char)i);
	return (i > 255 ? 255 : (unsigned char)i);
}
