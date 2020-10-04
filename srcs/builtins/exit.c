#include <minishell.h>
#include <stdlib.h>

int		ft_exit(t_term *term)
{
	//term_destroy(t); TO DO i supose it just free all the allocated stuff
	write(1, "exit\n", 5);
	tputs(term->caps.insert_end, 0, &ft_putchar); // i dunno men
	ft_dprintf(2, "exiting with status code %d", term->st);
	exit(term->st);
	return (term->st);
}
