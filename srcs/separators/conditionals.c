/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditionals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 00:01:31 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 22:01:04 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <separators.h>
#include <execution.h>

static void	handle_parentheses(int parser_st, int *flags, int *status)
{
	if (parser_st & CLOSE_PAR)
	{
		if ((*flags & SKIP && *flags & SKIPED_AND) || (!(*flags & SKIP) \
				&& !(*flags & NOT_SKIPED_OR)))
			*status = 42;
		*flags &= ~SKIP;
	}
}

int			handle_conditionals(int parser_st, int *flags, int parentheses_nb)
{
	int		status;

	status = g_session.st;
	if (!parentheses_nb)
		*flags &= ~SKIP;
	handle_parentheses(parser_st, flags, &status);
	if (parser_st & SEMICOLON)
		*flags &= ~SKIP;
	if (*flags & SKIP)
		return (false);
	if (parser_st & AND && status)
		return (!(*flags = SKIP | (parentheses_nb ? SKIPED_AND : 0)));
	if (parser_st & OR && !status)
	{
		*flags &= ~NOT_SKIPED_OR;
		return (!(*flags |= SKIP));
	}
	else if (parentheses_nb)
		*flags |= NOT_SKIPED_OR;
	if (!(*flags & SKIP))
		*flags = *flags & NOT_SKIPED_OR ? NOT_SKIPED_OR : NONE;
	return (true);
}
