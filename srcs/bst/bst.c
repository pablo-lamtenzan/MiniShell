/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:23:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 21:48:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>

#include <stdio.h> // debug

static t_bst*	build_job(t_tok* tokens, t_tok* delim)
{
    t_tok* 		tk1;
	t_tok*		tk2;
	t_bst*		node;

	tk1 = find_last_operator(tokens, delim);
    node = new_node(NULL, NULL, tk1->type);
	tk2 = find_next_operator(tokens, REDIR_GR | REDIR_LE | REDIR_DG | PIPE);
	if (tk1 != tk2 && tk2 != delim)
	{
		node->a = build_job(tokens, tk1);
		dprintf(2, "BST: filename is: [%s]\n", tk1->data);
        node->b = tk1->data;
	}
	else
	{
		if (tokens->type & CMD)
            node->a = /*new_node(tokens->data, NULL, CMD)*/ tokens->data;
		if (tk1->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		{
            node->b = /*new_node(tk1->data, NULL, FILENAME)*/ tk1->data;
			dprintf(2, "BST: filename is: [%s]\n", tk1->data);
		}
		node->type |= CMD;
	}
	return (node);
}

static t_bst*	build_bst(t_tok* tokens)
{
    t_tok*		tk1;
	t_tok*		tk2;
	t_tok*		tk3;
    t_bst*		node;

	if ((tk1 = find_next_operator(tokens, PIPE))->type & PIPE)
    	node = new_node(NULL, NULL, tk1->type);
	else if ((tk3 = find_next_operator(tokens, REDIR_GR | REDIR_LE \
             | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
        node = new_node(NULL, NULL, tk3->type);
	else
		node = new_node(NULL, NULL, CMD);
    node->a = build_job(tokens, tk1);
    if ((tk2 = find_next_operator(tk1->next, PIPE))->type & PIPE)
        node->b = build_bst(tk1->next);
	else if ((tk3 = find_next_operator(tk1, REDIR_GR | REDIR_LE \
             | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
			node->b = build_job(tk1->next, NULL);
	else
		node->b = build_job(tk1->next, NULL);
	return (node);
}

t_bst*			bst(t_tok* tokens)
{
	t_tok*		last_node;

	if (!(last_node = is_pipe_cmd(tokens)))
		return (build_bst(tokens));
	else if (tokens->next) // this 
		return (build_job(tokens, last_node->next));
	return (build_job(tokens, NULL)); // and this seems the same in all the cases
}

void			free_bst(t_bst* root)
{
	if (!root)
		return ;
	free_bst(root->a);
	free_bst(root->b);
	free(root);
}
