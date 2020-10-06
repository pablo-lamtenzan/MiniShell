#ifndef BUILTINS_H
# define BUILTINS_H

#include 	<minishell.h>
#include    <limits.h>

typedef struct  s_builtin_args
{
    int     fds[2];
    int     ac;
    char    **av;
    t_term  *t;
}               t_builtin_args;

typedef int	(*t_builtin)(t_builtin_args *);

int     	ft_cd(t_builtin_args *args);
int	        ft_echo(t_builtin_args *args);
int	        ft_env(t_builtin_args *args);
int			ft_exit(t_builtin_args *args);
int			ft_export(t_builtin_args *args);
int			ft_pwd(t_builtin_args *args);
int			ft_unset(t_builtin_args *args);

t_builtin	builtin_get(const char *name);

#endif
