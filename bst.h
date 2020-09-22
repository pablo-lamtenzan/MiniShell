#ifndef BST_H
# define BST_H

#include <minishell.h>
#include <stdbool.h>

#define NONE					0
#define PIPE					1
#define REDIRECTION_GREATHER	2
#define REDIRECTION_LESSER		4
#define REDIRECTION_DGREATHER	8
#define SEMICOLON				16

typedef struct					s_bst
{
	unsigned char				operator;
	char						*cmd[2]; // probally this need to be char ** or changed
	struct s_bst				*next;
	struct s_bst				*back;
}								t_bst;

t_bst							*new_node(const unsigned char operator, char *cmd[2], t_bst *back);
t_bst							*build_bst(t_token *operators, t_token **cmds);
void							execute_bst(t_bst *head, t_data *data);
bool							execute_simple_cmd(t_bst *curr, t_data *data);
bool							execute_redirections_cmd(t_bst *curr, t_data *data);

#endif