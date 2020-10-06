#include <minishell.h>
#include <stdlib.h>

int		ft_exit(t_builtin_args *args)
{
	term_destroy(args->t);
	write(1, "exit\n", 5);
	tputs(args->t->caps.insert_end, 0, &ft_putchar); // i dunno men
	ft_dprintf(2, "exiting with status code %d", args->t->st);
	exit(args->t->st);
	return (args->t->st);
}
