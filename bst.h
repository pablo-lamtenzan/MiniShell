#ifndef BST_H
# define BST_H

#define NONE					0
#define PIPE					1
#define REDIRECTION_GREATHER	2
#define REDIRECTION_LESSER		4
#define REDIRECTION_DGREATHER	8
#define SEMICOLON				16

typedef struct					s_bst
{
	unsigned char				operator;
	char						*cmd[2];
	struct s_bst				*next;
	struct s_bst				*back;
}								t_bst;

t_bst							*new_node(const unsigned char operator, char *cmd[2], t_bst *back);
t_bst							*build_bst(char *operators, char **cmds);

#endif