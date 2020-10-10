#ifndef BUILTINS_H
# define BUILTINS_H

# include <limits.h>

# include <term/term.h>
# include <executable.h>

int     	ft_cd(t_args *args, t_term *t);
int	        ft_echo(t_args *args, t_term *t);
int	        ft_env(t_args *args, t_term *t);
int			ft_exit(t_args *args, t_term *t);
int			ft_export(t_args *args, t_term *t);
int			ft_pwd(t_args *args, t_term *t);
int			ft_unset(t_args *args, t_term *t);

#endif
