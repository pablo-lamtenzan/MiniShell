#ifndef BST_H
# define BST_H

#include <stdbool.h>
#include <libft.h>
#include <term/term.h>
#include <stdint.h>
#include <cmd.h>

typedef struct		s_data
{
	int32_t			ac;
	int8_t			**argv;
	t_map			*env;
	t_caps			*caps;
	int32_t			pid;
	int8_t			return_status;
}					t_data;

typedef struct					s_bst
{
	t_operator_t				operator;
	char						**cmd[2];
	uint16_t					size_1[2];
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

t_bst							*new_node(const unsigned char operator, char **cmd[2], t_bst *back);
t_bst							*build_bst(t_token *operators, t_token *cmds);

void							execute_bst(t_bst *head, t_data *data);
bool							execute_simple_cmd(t_bst *curr, t_data *data);
bool							execute_redirections_cmd(t_bst *curr, t_data *data);
bool							execute_pipe_cmd(t_bst *curr, t_data *data);
bool							free_four_ptrs_and_bst(void **ptr1, void **ptr2, void **ptr3,
	void **ptr4, t_bst **bst);

t_token							*token_clear(t_token **tokens);
char**							token_tab(t_token *tokens, int *n);

#endif
