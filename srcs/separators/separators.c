/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 00:01:43 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 03:18:23 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <separators.h>
#include <execution.h>

// have to clear the separators here
t_tok*		handle_separators(t_tok** tokens, int* status, int* parentheses_nb)
{
	t_tok*			tk1;
	t_tok*			tk2;
	t_tok*			tk3;
	t_tok*			tk4;
	static int		saved = 0;

	// end 
	if (!*tokens)
		return (NULL);
	while ((*tokens)->type & OPEN_PAR)
	{
		*tokens = (*tokens)->next;
		(*parentheses_nb)++;
	}

	// use last operator
	if (saved)
	{
		*status = saved;
		saved = 0;
	}

	
	// get next separator type and store it for use it next iteration
	// get tk1: the address of the next separator
	saved |= (tk1 = find_next_operator(*tokens, AND | OR | OPEN_PAR | CLOSE_PAR | SEMICOLON))->type;
	tk4 = tk1;

	// handling close parentheses
	if (saved & CLOSE_PAR)
	{
		// handle close par followed by n close pars
		if (tk4->next && tk4->next->type & CLOSE_PAR) // can remove this if
			while (tk4->next && tk4->next->type & CLOSE_PAR)
			{
				(*parentheses_nb)--;
				tk4 = tk4->next;
			}

		// handle close par followed by AND or OR
		if (tk4->next && tk4->next->type & (AND | OR) && (tk4 = tk4->next))
			saved |= tk4->type;
	}

	// handling open parentheses
	else if (saved & (OR | AND))
	{
		// AND or OR followed by n opening parentheses
		if (tk4->next && tk4->next->type & OPEN_PAR) // can remove this if
			while (tk4->next && tk4->next->type & OPEN_PAR)
			{
				(*parentheses_nb)++;
				tk4 = tk4->next;
			}
		saved |= tk4->type;
	}
	

	// get the previous address of the next separator
	tk2 = *tokens;
	while (tk2->next && tk2->next != tk1)
		tk2 = tk2->next;

	/* HAVE TO TEST THIS BUT I M LAZY RIGHT NOW */
	// frees all the sepators execpt the last one (cuase last will be used)
	/*
	t_tok* tmp_test_free;
	while (tk1 != tk4 && tk1)
	{
		tmp_test_free = tk1;
		tk1 = tk1->next;
		free(tmp_test_free->data);
		free(tmp_test_free);
	}*/

	// put its next to NULL
	if (saved & (AND | OR | OPEN_PAR | CLOSE_PAR | SEMICOLON))
		tk2->next = NULL;

	// update the tokens list to the next of the next separator
	tk3 = *tokens;
	*tokens = tk4->next;
	/* ANOTHER TEST I HAVE TO DO... */
	// frees the last/only separator
	/*free(tk4);*/

	// return the list starting in the last tokens value (ended by NULL now)
	return (tk3);
}