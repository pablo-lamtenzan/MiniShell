#include <minishell.h>

int		ft_exit(t_data *data)
{
	//term_destroy(t); TO DO i supose it just free all the allocated stuff
	write(1, "exit\n", 5);
	tputs(t->caps.insert_end, 0, &ft_putchar); // i dunno men
	ft_dprintf(2, "exiting with status code %d", t->st);
	exit(t->st);
	return (t->st);
}
