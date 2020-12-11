/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:23:23 by pablo             #+#    #+#             */
/*   Updated: 2020/12/11 01:18:53 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>

static t_bst		*build_job(t_tok *tokens, t_tok *delim)
{
	t_tok			*tk1;
	t_tok			*tk2;
	t_bst			*node;

	tk1 = find_last_operator(tokens, delim);
	if (!(node = new_node(NULL, NULL, tk1->type)))
		return (NULL);
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

static t_bst		*build_bst(t_tok *tokens)
{
	t_tok			*tk1;
	t_tok			*tk2;
	t_bst			*node;

	if ((tk1 = find_next_operator(tokens, PIPE))->type & PIPE)
		node = new_node(NULL, NULL, tk1->type);
	else if ((tk2 = find_next_operator(tokens, REDIR_GR | REDIR_LE \
			| REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		node = new_node(NULL, NULL, tk2->type);
	else
		node = new_node(NULL, NULL, CMD);
	if (!node)
		return (NULL);
	node->a = build_job(tokens, tk1);
	if (find_next_operator(tk1->next, PIPE)->type & PIPE)
		node->b = build_bst(tk1->next);
	else if (find_next_operator(tk1, REDIR_GR | REDIR_LE \
			| REDIR_DG)->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		node->b = build_job(tk1->next, NULL);
	else
		node->b = build_job(tk1->next, NULL);
	return (node);
}

t_bst				*bst(t_tok *tokens)
{
	t_tok			*last_node;

	if (!(last_node = is_pipe_cmd(tokens)))
		return (build_bst(tokens));
	else if (tokens->next)
		return (build_job(tokens, last_node->next));
	return (build_job(tokens, NULL));
}

void				free_bst(t_bst *root)
{
	if (!root)
		return ;
	if (!(root->type & CMD))
		free_bst(root->a);
	if (root->type & PIPE)
		free_bst(root->b);
	free(root);
}
