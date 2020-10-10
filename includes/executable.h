#ifndef EXECUTABLE_H
# define EXECUTABLE_H

#include <term/term.h>

typedef struct  s_args
{
	int			fds[3];
	void		*exec;
	int			ac;
    char *const	*av;
    char *const	*ep;
}				t_args;

typedef int	(*t_executable)(t_args *args, t_term *term);


#endif
