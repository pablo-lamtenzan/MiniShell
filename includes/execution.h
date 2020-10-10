#ifndef EXECUTION_H
# define EXECUTION_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

# include <errno.h> // If this is allowed
# include <string.h>

# include <libft.h>

# include <bst.h>
# include <args.h>
# include <builtins.h>
# include <path.h>

# define READ				0
# define WRITE				1

# define EXEPT				in_fd < 0 ? 0 : 1
# define NEXT_NODE			in_fd == -1 ? curr : curr->next

typedef struct				s_builtin_exec
{
	int32_t*				fds;
	int32_t					ac;
	char**					av;
}							t_builtin_exec;

bool						exec_cmd(t_args *args, t_term *term);
bool						exec_pipe_cmd(t_bst *curr, t_term *term, int in_fd, int index);


bool	            		redir_fds(int* fds, const char* filepath, t_operator_t op);

bool						get_path_and_envp(char** execution_path, char*** envp, char* cmd_name, t_term* term);

bool						close_fds(int *fds);
bool						dup_stdio(int fds[3]);

# endif
