/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_param_quoted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

/*
** PARAM_SQUOTED
** chars preceded by backslash or chars wrapped in single quotes, escaped by
** backslash.
**
** '\'' ( char - \' )* '\''
*/
t_lex_err	lex_param_squoted(t_tok **tokens, t_lex_st *st, t_tok_t type)
{
	t_tok		*param;
	const char	*start;

	if (!(st->wait & TOK_SQUOTED))
	{
		if (*st->input != '\'')
			return (LEX_ENOMATCH);
		st->input++;
		st->wait |= TOK_SQUOTED;
	}
	start = st->input;
	while (*st->input != '\0' && *st->input != '\'')
		st->input++;
	if (start == st->input || *st->input != '\'')
		return (LEX_EWAIT);
	if (!(param = token_strndup(start, st->input - start, TOK_SQUOTED | type)))
		return (LEX_EALLOC);
	token_add_back(tokens, param);
	st->input++;
	st->wait &= ~TOK_SQUOTED;
	return (LEX_EOK);
}

/*
** PARAM_DQUOTED
** chars wrapped in double quotes, escaped by backslash.
**
** '"' ( ( char - ["\"\\"] ) | ( '\\' ["\"\\"] ) )* '"'
*/
t_lex_err	lex_param_dquoted(t_tok **tokens, t_lex_st *st, t_tok_t type)
{
	t_tok		*param;
	const char	*start;

	if (!(st->wait & TOK_DQUOTED))
	{
		if (*st->input != '"')
			return (LEX_ENOMATCH);
		st->input++;
		st->wait |= TOK_DQUOTED;
	}
	start = st->input;
	while (*st->input != '\0' && *st->input != '"')
	{
		st->input++;
		if (*st->input == '\\')
		{
			if (st->input != start && !(param = token_strndup(start,
				st->input - start, TOK_DQUOTED | type)))
				return (LEX_EALLOC);
			token_add_back(tokens, param);
			if (!(param = token_strndup(++st->input, 1, TOK_SQUOTED | type)))
				return (LEX_EALLOC);
			token_add_back(tokens, param);
			start = ++st->input;
		}
	}
	if (*st->input != '"')
		return (LEX_EWAIT);
	if (!(param = token_strndup(start, st->input - start, TOK_DQUOTED | type)))
		return (LEX_EALLOC);
	token_add_back(tokens, param);
	st->input++;
	st->wait &= ~TOK_DQUOTED;
	return (LEX_EOK);
}

/*
** PARAM_QUOTED
** PARAM_SQUOTED or PARAM_DQUOTED.
**
** PARAM_SQUOTED | PARAM_DQUOTED
*/
t_lex_err	lex_param_quoted(t_tok **tokens, t_lex_st *st, t_tok_t type)
{
	t_lex_err	status;

	if (st->wait & TOK_SQUOTED
	|| (status = lex_param_dquoted(tokens, st, type)) == LEX_ENOMATCH)
		status = lex_param_squoted(tokens, st, type);
	return (status);
}
