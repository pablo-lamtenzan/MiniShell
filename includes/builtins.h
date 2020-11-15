#ifndef BUILTINS_H
# define BUILTINS_H

# include <limits.h>

# include <term/term.h>
# include <execution.h>

t_exec_status     	    ft_cd(t_exec *args, t_term *t);
t_exec_status	        ft_echo(t_exec *args, t_term *t);
t_exec_status	        ft_env(t_exec *args, t_term *t);
t_exec_status			ft_exit(t_exec *args, t_term *t);
t_exec_status			ft_export(t_exec *args, t_term *t);
t_exec_status			ft_pwd(t_exec *args, t_term *t);
t_exec_status			ft_unset(t_exec *args, t_term *t);

#endif
