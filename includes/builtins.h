#ifndef BUILTINS_H
# define BUILTINS_H

#include 	<minishell.h>

int			ft_cd(int ac, char **argv, t_data *data);
int			ft_echo(int ac, char **argv);
//int			ft_env(t_map *env);
int			ft_exit(t_data *data);
int			ft_export(int ac, char **av, t_data *data);
int			ft_pwd(void);
int			ft_unset(int ac, char **av, t_data *data);

#endif