/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 23:28:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 20:37:56 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// put this in a header
# define SKIPED_AND		1
# define SKIPED_OR		2
# define PST_OR			4 // change
# define PST_AND		8 // change
# define SKIP			16
# define PST_SEM		32
# define PARENTHESES	64
# define PST_PAREN_OPEN	128
# define PST_PARENT_CLOSE 256

/*
	Check if the execution can continue or not.
	- Handles parentheses
	- Works only with this structure [term->st][COND][ITERATES HERE]
	- Needs to know the separator who was before
	- If there are ")"or"(" + operator" need to know both
*/

static int	handle_conditionals(t_term **term, int parser_st, int *flags)
{
	static unsigned int nb_parentheses = 0;

	// handling a closing parentheses
	if (parser_st & PST_PARENT_CLOSE)
	{
		nb_parentheses--;
		// skip + and in end parentheses = false parentheses
		// !skip + or in end parentheses = false parentheses
		if ((*flags & SKIP && *flags & SKIPED_AND) || !(*flags & (SKIP | SKIPED_OR)))
		{
			(*term)->st = 127; // overwrite false
			*flags &= ~SKIP;
		}
	}
	// handling an opening parentheses
	if (parser_st & PST_PAREN_OPEN)
		nb_parentheses++;

	// if semicolon reset flags
	if (parser_st & PST_SEM)
		*flags &= ~SKIP; // here flags = 0, a reset
		// semicolon in paretheses error
	
	// some conditional in the same conditional scope returned false
	if (*flags & SKIP)
		return (false);

	// conditionals errors
	if (parser_st & PST_AND && (*term)->st) // false && something
		return (!(*flags = SKIP | (nb_parentheses ? SKIPED_AND : 0))); // deletes all bits execp skip
	if (parser_st & PST_OR && !(*term)->st) // true || something
		return (!(*flags |= SKIP | (nb_parentheses ? SKIPED_OR : 0))); // deletes all bits execp skip

	// if condionals error wasn't been trown reset
	if (!(*flags & SKIP)) 
		*flags = NONE;

	return (true);
}

# define AND		1
# define OR			2
# define OPEN_PAR	4
# define CLOSE_PAR	8
# define SEMICOLON	16

static t_tok*		handle_separators(t_tok** tokens, int* status)
{
	t_tok*			tk1;
	t_tok*			tk2;
	t_tok*			tk3;
	t_tok*			tk4;
	static int		saved = 0;

	// end 
	if (!*tokens)
		return (NULL);

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
				tk4 = tk4->next;

		// handle close par followed by AND or OR
		else if (tk4->next && tk4->next->type & (AND | OR) && (tk4 = tk4->next))
			saved |= tk4->type;
	}

	// handling open parentheses
	else if (saved & (OR | AND))
	{
		// AND or OR followed by n opening parentheses
		if (tk4->next && tk4->next->type & OPEN_PAR)
			while (tk4->next && tk4->next->type & OPEN_PAR)
				tk4 = tk4->next;
		saved |= tk4->type;
	}
	

	// get the previous address of the next separator
	tk2 = *tokens;
	while (tk2->next && tk2->next != tk1)
		tk2 = tk2->next;

	// put its next to NULL
	tk2->next = NULL;

	// update the tokens list to the next of the next separator
	tk3 = *tokens;
	*tokens = tk4->next;

	// return the list starting in the last tokens value (ended by NULL now)
	return (tk3);
}

static int	exec(t_tok* tokens, t_term* term)
{
	tokens_print(tokens);
	return (true);
    int		status;
	t_tok*	exec_tokens;
	t_bst*	root;
	int		conditionals;

	conditionals = NONE;
	while ((exec_tokens = handle_separators(&tokens, &status)))
	{
		if (handle_conditionals(&term, status, &conditionals))
		{
			execute_bst(root = bst(exec_tokens), term);
			free_bst(root);
		}
		free(exec_tokens);
		exec_tokens = NULL;
	}
	//free_all(term); // TODO, input to be freed ?
	if (status == -1) // change to ERROR define
		return (false);
	return (true);
}


int			main(int ac, const char** av, const char** envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	
	tests();
	if (0)
		exec(NULL, NULL);
    //status = term_prompt(ac, av, envp, &exec);
    // TODO: free everything
    return (false && term_prompt(ac, av, envp, &exec);
}