#ifndef BUILTINS_H
# define BUILTINS_H

#include 	<minishell.h>
#include    <limits.h>

int			ft_cd(int ac, const char **av, t_data *data);
int			ft_echo(int ac, const char **av);
int			ft_env(t_map *env);
int			ft_exit(t_data *data);
int			ft_export(int ac, const char **av, t_data *data);
int			ft_pwd(void);
int			ft_unset(int ac, const char **av, t_data *data);

#endif
