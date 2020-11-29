#ifndef BUILTINS_H
# define BUILTINS_H

/*
** External
*/
# include <limits.h>

/*
** Local
*/
# include <term/term.h>
# include <execution.h>

/*
** Subject builtins
*/
int			ft_cd(t_exec *args);
int			ft_echo(t_exec *args);
int			ft_env(t_exec *args);
int			ft_exit(t_exec *args);
int			ft_export(t_exec *args);
int			ft_pwd(t_exec *args);
int			ft_unset(t_exec *args);

/*
** Job control builtins
*/
int			ft_fg(t_exec *args);
int			ft_bg(t_exec *args);
int			ft_kill(t_exec *args);
int			ft_jobs(t_exec *args);
int			ft_disown(t_exec *args);
int			ft_wait(t_exec *args);

#endif
