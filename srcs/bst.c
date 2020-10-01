/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 17:23:02 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/23 17:28:44 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <stdlib.h>

t_bst		*new_node(const unsigned char operator, char *cmd[2], t_bst *back)
{
	t_bst	*new;

	if (!(new = (t_bst *)malloc(sizeof(t_bst))))
		return (NULL);
	new->next = NULL;
	if (back)
		back->next = new;
	new->back = back;
	new->cmd[0] = cmd[0];
	new->cmd[1] = cmd[1];
	new->operator = operator;
	return (new);
}

/* I want to use this function to create a cmd, each time a semicolon
		will be parsed, a new call of this function will be done.
		Number of bst = Number of semicolons + 1
*/
t_bst		*build_bst(t_token *operators, t_token *cmds)
{
	t_bst	*tail;
	t_bst	*head;
	char	*cmds_format_conv[2]; // cmds->data has to be malloc
	int		it[2];

	tail = NULL;
	it[0] = 0;
	it[1] = 0;
	while (operators)
	{
		if (tail)
		{
			cmds_format_conv[0] = NULL;
			cmds_format_conv[1] = cmds->data;
			cmds = cmds->next;
		}
		else
		{
			cmds_format_conv[0] = cmds->data;
			cmds = cmds->next;
			cmds_format_conv[1] = cmds->data;
			cmds = cmds->next;
			head = tail;
		}
		if (!(tail = new_node(operators->type, cmds_format_conv, tail))
				|| cmds == NULL)
			return (NULL);
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

void		execute_bst(t_bst *head, t_data *data)
{
	if (head->operator & NONE)
		execute_simple_cmd(head, data);
	else
	{
		/* The idea is start here with his fcts and if after a pipe there is another pipe or
			an redirection the function execute_pipe must handle this.
		*/
		if (head->operator & PIPE)
			execute_pipe_cmd(head, data);
		/* This function will be called once if there are a redirection in the first node*/
		if (head->operator & REDIRECTION_GREATHER || head->operator & REDIRECTION_LESSER
			|| head->operator & REDIRECTION_DGREATHER)
			execute_redirections_cmd(head, data);
	}
}
/* The previous function will call the executers functions (calling execve or calling a function
	who calls execve)
*/