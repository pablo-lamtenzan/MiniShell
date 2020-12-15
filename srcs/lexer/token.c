/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by chamada           #+#    #+#             */
/*   Updated: 2020/12/11 17:31:54 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/token.h>

t_tok		*token_new(void *data, t_tok_t type)
{
	t_tok	*token;

	if (!(token = malloc(sizeof(*token))))
		return (NULL);
	token->next = NULL;
	token->data = data;
	token->type = type;
	return (token);
}

t_tok		*token_strndup(const char *input, size_t n, t_tok_t type)
{
	t_tok	*token;
	char	*content;

	if (!(content = ft_strndup(input, n)))
		return (NULL);
	if (!(token = token_new(content, type)))
	{
		free(content);
		return (NULL);
	}
	return (token);
}

void		token_add_back(t_tok **tokens, t_tok *token)
{
	t_tok	*current;

	if (token)
	{
		if (*tokens)
		{
			current = *tokens;
			while (current->next)
				current = current->next;
			current->next = token;
		}
		else
			*tokens = token;
	}
}

void		token_add_front(t_tok **tokens, t_tok *token)
{
	token->next = *tokens;
	*tokens = token;
}

/*
** Clear a token list recursively depending on token type.
**
** type & TOK_CMD and type == TOK_PARAM are cleared recursively.
*/

void		token_clr(t_tok **tokens)
{
	t_tok	*curr;

	while ((curr = *tokens))
	{
		if (curr->type & TOK_CMD || (curr->type == TOK_PARAM))
			token_clr((t_tok**)&curr->data);
		else
			free(curr->data);
		*tokens = curr->next;
		free(curr);
	}
}
