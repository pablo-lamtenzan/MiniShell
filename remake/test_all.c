/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 23:28:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/02 01:16:02 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

// put this in a header
# define NONE			0
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
	{
		*flags &= ~SKIP; // here flags = 0, a reset
		// semicolon in paretheses error
		if (nb_parentheses && parser_st & PST_SEM)
			return (-1); // fatal error
	}
	
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

static int	exec(const char* input, t_term* term)
{
    int		status;
	t_tok*	clems_stuff;
	t_bst*	root;
	int		conditionals;

	conditionals = NONE;
	while ((status = clements_stuff(&clems_stuff)))
	{
		if (handle_conditionals(term->st, status, &conditionals))
		{
			execute_bst((root = bst(clems_stuff)), term);
			free_bst(root);
		}
		free(clems_stuff);
		clems_stuff = NULL;
	}
	free_all(term); // TODO, input to be freed ?
	if (status == -1) // change to ERROR define
		return (false);
	return (true);
}

int			main(int ac, const char** av, const char** envp)
{
    int		status;

    status = term_prompt(ac, av, envp, &exec);
    // TODO: free everything
    return (status);
}