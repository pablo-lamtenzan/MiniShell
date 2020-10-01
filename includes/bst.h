#ifndef BST_H
# define BST_H

#include <stdbool.h>
#include <libft.h>
#include <term/term.h>

#define NONE					0
#define PIPE					1
#define REDIRECTION_GREATHER	2
#define REDIRECTION_LESSER		4
#define REDIRECTION_DGREATHER	8
#define SEMICOLON				16

typedef struct		s_data
{
	int				ac;
	char			**argv;
	t_map			*env;
	t_caps			*caps;
	int				pid;
	int				return_status;
}					t_data;

typedef struct					s_bst
{
	unsigned char				operator;
	char						**cmd[2]; // probally this need to be char ** or changed
	size_t						size_1;
	size_t						size_2;
	struct s_bst				*next;
	struct s_bst				*back;
}								t_bst;

typedef struct		s_token
{
	char			**data;
	unsigned char	type;
	struct s_token	*next;
}					t_token;

typedef struct  	s_pipe
{
	int				fd[2];
	int				fd_read;
	int				fd_write;
	int				in;
	int				out;
}					t_pipe;

typedef struct		s_pipe2
{
	int				in[2];
	int				fd[2];
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
