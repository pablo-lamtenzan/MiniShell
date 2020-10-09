/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 17:23:02 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/09 17:15:40 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <stdlib.h>

static void			assign_values(t_bst* target, t_cmd* cmds[2])
{
	if (cmds[0])
	{
		target->ac[0] = cmds[0]->ac;
		target->av[0] = cmds[0]->av;
	}
	else
	{
		target->ac[0] = 0;
		target->av[0] = NULL;
	}
	if (cmds[1])
	{
		target->ac[1] = cmds[1]->ac;
		target->av[1] = cmds[1]->av;
	}
	else
	{
		target->ac[1] = 0;
		target->av[1] = NULL;
	}
}

static t_bst*		new_node(const t_operator_t operator, t_cmd* cmds[2],
		t_bst* back)
{
	t_bst*			new;

	if (!(new = (t_bst *)malloc(sizeof(t_bst))))
		return (NULL);
	new->next = NULL;
	if (back)
		back->next = new;
	new->back = back;
	assign_values(new, cmds);
	new->operator = operator;
	return (new);
}

t_bst*				build_bst(t_operator* operators, t_cmd* cmds)
{
	t_bst*			tail;
	t_bst*			head;
	t_cmd*			cmds_conv[2];

	head = NULL;
	tail = NULL;
	while (operators)
	{
		cmds_conv[0] = NULL;
		cmds_conv[1] = NULL;
		if (tail)
			cmds_conv[1] = cmds;
		else if (cmds && (cmds_conv[0] = cmds) \
				&& (cmds = cmds->next))
		{
			cmds_conv[1] = cmds;
			cmds = cmds->next;
			head = tail;
		}
		if (!(tail = new_node(operators->type, cmds_conv, tail)))
			return (NULL);
		if (!head)
			head = tail;
		operators = operators->next;
	}
	return (head);
}

void				execute_bst(t_bst* head, t_term* term)
{
	if (head->operator == NONE)
		execute_simple_cmd(head, term);
	else
	{
		if (head->operator & PIPE)
			execute_pipes_cmd(INIT, head, term);
		if (head->operator & REDIR_GR \
				|| head->operator & REDIR_LE \
				|| head->operator & REDIR_DG)
			execute_redirections_cmd(head, term);
	}
}

