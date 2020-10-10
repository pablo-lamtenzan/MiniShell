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
	char						**av[2];
	uint32_t					ac[2];
	struct s_bst				*next;
	struct s_bst				*back;
}								t_bst;

t_bst							*build_bst(t_operator *operators, t_cmd *cmds);
bool							execute_bst(t_bst *head, t_term *term);
bool							free_ptrs_and_bst(void *ptr1, void *ptr2, t_bst **bst);
bool							free_one_ptr(void *ptr);
bool							free_bst_node(t_bst **bst);

#endif
