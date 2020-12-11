/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 00:01:43 by pablo             #+#    #+#             */
/*   Updated: 2020/12/11 22:15:54 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <separators.h>
#include <execution.h>

static t_tok	*increment(t_tok **target)
{
	t_tok		*next;

	next = (*target)->next;
	free(*target);
	*target = next;
	return (*target);
}

static void		skip_parentheses(t_tok **tks, int *parentheses_nb, int *saved)
{
	if (*saved & CLOSE_PAR)
	{
		while (tks[3]->next && tks[3]->next->type & CLOSE_PAR \
				&& increment(&tks[3]))
			(*parentheses_nb)--;
		if (tks[3]->next && tks[3]->next->type & (AND | OR) \
				&& increment(&tks[3]))
			*saved |= tks[3]->type;
	}
	else if (*saved & (OR | AND))
	{
		while (tks[3]->next && tks[3]->next->type & OPEN_PAR \
				&& increment(&tks[3]))
			(*parentheses_nb)++;
		*saved |= tks[3]->type;
	}
}

t_tok			*handle_separators(t_tok **tokens, int *status,
		int *parentheses_nb)
{
	t_tok		*tks[4];
	static int	saved = 0;

	if (!*tokens)
		return (NULL);
	while ((*tokens)->type & OPEN_PAR && increment(tokens))
		(*parentheses_nb)++;
	if (saved && (*status = saved))
		saved = 0;
	tks[0] = find_next_operator(*tokens, AND | OR | OPEN_PAR \
			| CLOSE_PAR | SEMICOLON);
	saved |= tks[0]->type;
	tks[3] = tks[0];
	skip_parentheses(tks, parentheses_nb, &saved);
	tks[1] = *tokens;
	while (tks[1] && tks[1]->next && tks[1]->next != tks[0])
		tks[1] = tks[1]->next;
	if (tks[1] && saved & (AND | OR | OPEN_PAR | CLOSE_PAR | SEMICOLON))
		tks[1]->next = NULL;
	tks[2] = *tokens;
	*tokens = tks[3]->next;
	if (tks[3]->type & (AND | OR | OPEN_PAR | CLOSE_PAR | SEMICOLON))
		free(tks[3]);
	return (tks[2]);
}
