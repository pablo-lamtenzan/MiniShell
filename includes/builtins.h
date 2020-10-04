#ifndef BUILTINS_H
# define BUILTINS_H

#include 	<minishell.h>
#include    <limits.h>

int			ft_cd(int ac, char* *av, t_term *term);
int			ft_echo(int ac, char* *av);
int			ft_env(t_map *env);
int			ft_exit(t_term *term);
int			ft_export(int ac, char* *av, t_term *term);
int			ft_pwd(void);
int			ft_unset(int ac, char* *av, t_term *term);

#endif
