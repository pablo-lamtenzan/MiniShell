#ifndef BUILTINS_H
# define BUILTINS_H

# include <limits.h>

# include <term/term.h>
# include <execution.h>

/*
** Subject builtins
*/
int     	ft_cd(t_exec *args, t_term *t);
int	        ft_echo(t_exec *args, t_term *t);
int	        ft_env(t_exec *args, t_term *t);
int			ft_exit(t_exec *args, t_term *t);
int			ft_export(t_exec *args, t_term *t);
int			ft_pwd(t_exec *args, t_term *t);
int			ft_unset(t_exec *args, t_term *t);

/*
** Job control builtins 
*/
int		    ft_fg(t_exec* args, t_term* term);
int			ft_bg(t_exec* args, t_term* term);
int     	ft_kill(t_exec* args, t_term* term);

#endif
