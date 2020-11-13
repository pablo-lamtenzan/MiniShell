/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:23:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 20:52:18 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>

static t_bst*	build_job(t_tok* tokens, t_tok* delim)
{
	t_tok*	tk1;
	t_tok*	tk2;
	t_bst*	node;

	tk1 = find_last_operator(tokens, delim);
	node = new_node(NULL, NULL, tk1->type);
	tk2 = find_next_operator(tokens, REDIR_GR | REDIR_LE | REDIR_DG | PIPE);
	if (tk1 != tk2 && tk2 != delim)
	{
		node->a = build_job(tokens, tk1);
		node->b = tk1->data;
	}
	else
	{
		if (tokens->type & CMD)
			node->a = tokens->data;
		if (tk1->type & (REDIR_GR | REDIR_LE | REDIR_DG))
			node->b = tk1->data;
		node->type |= CMD;
	}
	return (node);
}

static t_bst*	build_bst(t_tok* tokens)
{
	t_tok*	tk1;
	t_tok*	tk2;
	t_tok*	tk3;
	t_bst*	node;

	if ((tk1 = find_next_operator(tokens, PIPE))->type & PIPE)
		node = new_node(NULL, NULL, tk1->type);
	else if ((tk3 = find_next_operator(tokens, REDIR_GR | REDIR_LE \
			 | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		node = new_node(NULL, NULL, tk3->type);
	else
		node = new_node(NULL, NULL, CMD);
	node->a = build_job(tokens, tk1);
	if ((tk2 = find_next_operator(tk1->next, PIPE))->type & PIPE)
		node->b = build_bst(tk1->next); // TODO: dead assignment tk2 (unused)
	else if ((tk3 = find_next_operator(tk1, REDIR_GR | REDIR_LE \
			 | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
			node->b = build_job(tk1->next, NULL); // TODO: dead assignment tk3 (unused)
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

void			print_bst(t_bst* root, int type)
{
	if (!root)
		return ;
	if (type == 0)
		ft_dprintf(2, "[PRINT BST: LEFT BRANCH:  addr[%p],type:[%d]\n", root, root->type);
	if (type == 1)
		ft_dprintf(2, "[PRINT BST: RIGHT BRANCH: addr[%p],type:[%d]\n", root, root->type);
	if (!(root->type & CMD))
		print_bst(root->a, 0);
	if (root->type & PIPE)
		print_bst(root->b, 1);
}

void			free_bst(t_bst* root)
{
	if (!root)
		return ;
	ft_dprintf(2, "[FREE BST: addr[%p],type:[%d]\n", root, root->type);
	if (!(root->type & CMD))
		free_bst(root->a);
	if (root->type & PIPE)
		free_bst(root->b);
	free(root);
}
