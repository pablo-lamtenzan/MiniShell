/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 17:23:02 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/06 19:45:37 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <stdlib.h>

static t_bst		*new_node(const t_operator_t operator, int ac[2], char* *cmd[2], t_bst *back)
{
	t_bst	*new;

	if (!(new = (t_bst *)malloc(sizeof(t_bst))))
		return (NULL);
	new->next = NULL;
	if (back)
		back->next = new;
	new->back = back;
	new->ac[0] = ac[0];
	new->ac[1] = ac[1];
	new->av[0] = cmd[0];
	new->av[1] = cmd[1];
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
	char**		cmds_format_conv[2];
	int			ac[2];
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
		if (tail)
		{
			cmds_format_conv[0] = NULL;
			cmds_format_conv[1] = cmds->av;
			ac[0] = 0;
			ac[1] = cmds->ac;
			cmds = cmds->next;
		}
		else
		{
			if (cmds)
			{
				cmds_format_conv[0] = cmds->av;
				ac[0] = cmds->ac;
				
				//ft_printf("[DBG] av[0][0] value is %s\n", cmds_format_conv[0][0]);
				//ft_printf("[DBG] av[0][1] value is %s\n", cmds_format_conv[0][1]);
				cmds = cmds->next;
				if (cmds)
				{
					cmds_format_conv[1] = cmds->av;
					ac[1] = cmds->ac;
					cmds = cmds->next;
					head = tail;
				}
			}
			
		}
		if (!(tail = new_node(operators->type, ac, cmds_format_conv, tail))
				/*|| cmds == NULL*/)
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
			free_bst_node(&head);
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
