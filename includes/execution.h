#ifndef EXECUTION_H
# define EXECUTION_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

# include <libft.h>

# include <bst.h>
# include <builtins.h>
# include <path.h>

bool				exec_builtin(t_builtin_args *args);
bool	            redir_fds(int *fds, const char *filepath, t_operator_t op);

# endif
