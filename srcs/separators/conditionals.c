/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditionals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 00:01:31 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 05:27:07 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <separators.h>
#include <execution.h>

int	handle_conditionals(t_term** term, int parser_st, int* flags, int parentheses_nb)
{
	static unsigned int nb_parentheses = 0;

	// handling a closing parentheses
	if (parser_st & CLOSE_PAR)
	{
		nb_parentheses = parentheses_nb;
		// skip + and in end parentheses = false parentheses
		// !skip + or in end parentheses = false parentheses
		if ((*flags & SKIP && *flags & SKIPED_AND) || (!(*flags & SKIP) && !(*flags & NOT_SKIPED_OR)))
			(*term)->st = 127; // overwrite false
		*flags &= ~SKIP;
	}
	// handling an opening parentheses
	if (parser_st & OPEN_PAR)
		nb_parentheses = parentheses_nb;

	// if semicolon reset flags
	if (parser_st & SEMICOLON)
		*flags &= ~SKIP; // here flags = 0, a reset
		// semicolon in paretheses error
	
	// some conditional in the same conditional scope returned false
	if (*flags & SKIP)
		return (false);

	// conditionals errors
	if (parser_st & AND && (*term)->st) // false && something
		return (!(*flags = SKIP | (nb_parentheses ? SKIPED_AND : 0))); // deletes all bits execp skip
	if (parser_st & OR && !(*term)->st) // true || something
	{
		*flags &= ~NOT_SKIPED_OR;
		return (!(*flags |= SKIP)); // deletes all bits execp skip
	}
	else if (nb_parentheses)
		*flags |= NOT_SKIPED_OR; // skiped or is now not skiped or

	// if condionals error wasn't been trown reset
	if (!(*flags & SKIP)) 
		*flags = *flags & NOT_SKIPED_OR ? NOT_SKIPED_OR : NONE;

	return (true);
}