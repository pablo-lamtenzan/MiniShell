/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

/*
** SCOPE_IN
** Opening parenthesis.
**
** '('
*/

static t_lex_err	lex_scope_in(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;
	t_tok		*scope_in;

	status = LEX_ENOMATCH;
	if (*st->input == '(')
	{
		st->input++;
		st->subshell_level++;
		if ((scope_in = token_new(NULL, TOK_SCOPE_IN)))
		{
			token_add_back(tokens, scope_in);
			status = LEX_EOK;
		}
		else
			status = LEX_EALLOC;
	}
	return (status);
}

/*
** SCOPE_OUT
** Closing parenthesis.
**
** ')'
*/

static t_lex_err	lex_scope_out(t_tok **tokens, t_lex_st *st)
{
	t_tok	*scope_out;


	if (*st->input == ')')
	{
		if (st->subshell_level == 0)
			return (LEX_ESYNTAX);
		st->input++;
		st->subshell_level--;
		if (!(scope_out = token_new(NULL, TOK_SCOPE_OUT)))
			return (LEX_EALLOC);
		token_add_back(tokens, scope_out);
		return (LEX_EOK);
	}
	return (LEX_ENOMATCH);
}

/*
** SUBSHELL
** TOKENS wrapped in parenthesis.
**
** SCOPE_IN TOKENS SCOPE_OUT
*/

t_lex_err	lex_subshell(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;

	if ((status = lex_scope_in(tokens, st)) == LEX_EOK
	&& ((status = lex_tokens(tokens, st)) == LEX_EOK || status == LEX_EEND))
		status = lex_scope_out(tokens, st);
	return (status);
}
