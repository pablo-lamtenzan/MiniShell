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

# define READ				0
# define WRITE				1
# define INIT				-1

# define EXEPT				in_fd == INIT ? 0 : 1
# define NEXT_NODE			in_fd == INIT ? curr : curr->next

typedef struct				s_builtin_exec
{
	int32_t*				fds;
	int32_t					ac;
	char**					av;
}							t_builtin_exec;

bool						execute_simple_cmd(t_bst* curr, t_term* data);
bool						execute_redirections_cmd(t_bst* curr, t_term* data);
bool						execute_pipe_cmd(t_bst* curr, t_term* data);
bool						exec_builtin(t_builtin_args* args);
bool	            		redir_fds(int* fds, const char* filepath, t_operator_t op);
bool						execute_pipes_cmd(int in_fd, t_bst* curr, t_term* term);
bool						if_builtin_exec(const t_builtin_exec b, t_term* term, int32_t fds[3]);
bool						get_path_and_envp(char** execution_path, char*** envp, char* cmd_name, t_term* term);
bool						open_and_dup_stdio(t_bst *curr);
void						close_fds(int* fds);
bool						dup_stdio(int fds[3]);

# endif
