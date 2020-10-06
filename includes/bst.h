#ifndef BST_H
# define BST_H

#include <stdbool.h>
#include <libft.h>
#include <term/term.h>
#include <stdint.h>
#include <cmd.h>

typedef struct					s_bst
{
	t_operator_t				operator;
	char*					*av[2];
	uint32_t					ac[2];
	struct s_bst				*next;
	struct s_bst				*back;
}								t_bst;

/*
typedef struct  	s_pipe
{
	int				fd[2];
	int				fd_read;
	int				fd_write;
	int				in;
	int				out;
}					t_pipe;
*/

typedef struct		s_pipe2
{
	int8_t			in[2];
	int32_t			fd[2];
}					t_pipe2;

t_bst							*new_node (unsigned char operator, char **cmd[2], t_bst *back);
t_bst							*build_bst(t_operator *operators, t_cmd *cmds);
void							execute_bst(t_bst *head, t_term *data);
bool							execute_simple_cmd(t_bst *curr, t_term *data);
bool							execute_redirections_cmd(t_bst *curr, t_term *data);
bool							execute_pipe_cmd(t_bst *curr, t_term *data);
bool							free_ptrs_and_bst(void *ptr1, void *ptr2, t_bst **bst);
bool							free_one_ptr(void *ptr);
bool							free_bst_node(t_bst **bst);

bool							exec_builtin(int ac, char* *argv, t_term *data);
bool							get_path_and_envp(char **execution_path, char***envp, char *cmd_name, t_term *term);

#endif
