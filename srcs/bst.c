/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 17:23:02 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/07 01:50:11 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <stdlib.h>

static t_bst		*new_node(const t_operator_t operator, t_cmd *cmds[2], t_bst *back)
{
	t_bst	*new;

	if (!(new = (t_bst *)malloc(sizeof(t_bst))))
		return (NULL);
	new->next = NULL;
	if (back)
		back->next = new;
	new->back = back;
	if (cmds[0])
	{
		new->ac[0] = cmds[0]->ac;
		new->av[0] = cmds[0]->av;
	}
	else
	{
		new->ac[0] = 0;
		new->av[0] = NULL;
	}
	if (cmds[1])
	{
		new->ac[1] = cmds[1]->ac;
		new->av[1] = cmds[1]->av;
	}
	else
	{
		new->ac[1] = 0;
		new->av[1] = NULL;
	}
	new->operator = operator;
	return (new);
}

/* I want to use this function to create a cmd, each time a semicolon
		will be parsed, a new call of this function will be done.
		Number of bst = Number of semicolons + 1
*/
t_bst			*build_bst(t_operator *operators, t_cmd *cmds)
{
	t_bst		*tail;
	t_bst		*head;
	t_cmd		*cmds_conv[2];
	int			it[2];

	head = NULL;
	tail = NULL;
	it[0] = 0;
	it[1] = 0;
	if (!operators)
		ft_dprintf(2, "operators null address in build_bst\n");
	//ft_printf("Address of operator is %p\n", operators);
	//ft_printf("Type of operator is %i\n", operators->type);
	while (operators)
	{
		cmds_conv[0] = NULL;
		cmds_conv[1] = NULL;
		if (tail)
			cmds_conv[1] = cmds;
		else if (cmds)
		{
			cmds_conv[0] = cmds;
			if ((cmds = cmds->next))
			{
				cmds_conv[1] = cmds;
				cmds = cmds->next;
				head = tail;
			}
		}
		if (!(tail = new_node(operators->type, cmds_conv, tail)))
			return (NULL);
		if (!head)
			head = tail;
		operators = operators->next;
	}
	return (head);
}
/* The previous fct should return the head of the execution,
	the execution is composed by a binary search tree. The first node
	should be composed by an operator and 2 cmds (1 before and 1 after).
	If there are more than 1 node the next nodes should have only 1 cmd beacuse
	the other is the previous execution.

	If this is right, the engine is built in this function.
*/

void			execute_bst(t_bst *head, t_term *term)
{
	if (head->operator == NONE)
		execute_simple_cmd(head, term);
	else
	{
		/* The idea is start here with his fcts and if after a pipe there is another pipe or
			an redirection the function execute_pipe must handle this.
		*/
		if (head->operator & PIPE)
		{
			execute_pipe_cmd(head, term);
			//free_bst_node(&head);
		}
		/* This function will be called once if there are a redirection in the first node*/
		if (head->operator & REDIR_GR || head->operator & REDIR_LE
			|| head->operator & REDIR_DG)
			execute_redirections_cmd(head, term);
	}
}
/* The previous function will call the executers functions (calling execve or calling a function
	who calls execve)
*/
