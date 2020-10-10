#ifndef ARGS_H
# define ARGS_H

typedef struct  s_args
{
	int			fds[3];
	void		*exec; // can be t_builtin or const char *
	int			ac;
    char *const	*av;
    char *const	*ep;
}				t_args;

#endif
