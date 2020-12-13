/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

/*
** SEP
** Semicolon or new-line
**
** ';' | '\n'
*/
static t_lex_err	lex_sep(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;
	t_tok		*sep;

//	ft_dprintf(2, "[LEX][  SEP] Input: '%s'\n", st->input);
	if (*st->input == '\0')
		status = LEX_EEND;
	else if (*st->input == ';' || *st->input == '\n')
	{
		st->wait &= ~TOK_SEP;
		st->input++;
		if ((sep = token_new(NULL, TOK_SEP)))
		{
			token_add_back(tokens, sep);
			status = LEX_EOK;
		}
		else
			status = LEX_EALLOC;
	}
	else
		status = LEX_ENOMATCH;
	return (status);
}

/*
** TOKEN
** Command followed by multiple operations
**
** CMD (IFS OPERATION)*
*/
static t_lex_err	lex_token(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;

//	ft_dprintf(2, "[LEX][TOKEN] Input: '%s'\n", st->input);
	if ((status = lex_cmd(tokens, st)) == LEX_EOK)
	{
		while ((status = lex_ifs(st)) == LEX_EOK
		&& (status = lex_operation(tokens, st)) == LEX_EOK)
			;
		if (status == LEX_ENOMATCH)
			status = LEX_EOK;
	}
	return (status);
}

/*
** TOKENS
** TOKENs separated by SEP.
**
** ( IFS TOKEN IFS SEP )*
*/
t_lex_err			lex_tokens(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;

	/* if (st->wait & TOK_CMD && (status = lex_cmd(st)) != LEX_EOK)
		return (status); */

	while ((status = lex_ifs(st)) == LEX_EOK
	&& (status = lex_token(tokens, st)) == LEX_EOK
	&& (status = lex_ifs(st)) == LEX_EOK
	&& (status = lex_sep(tokens, st)) == LEX_EOK)
		;
	/* ft_dprintf(2, "[LEX][TOKNS] Status: %d, Wait: %d, Subsh-lvl: %d\n",
		status, st->wait, st->subshell_level); */
	if (status == LEX_ENOMATCH)
		status = LEX_ESYNTAX;
	else if (status == LEX_EEND && st->subshell_level == 0)
		status = LEX_EOK;
	return (status);
}
