/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_inline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

/*
** REDIR_OPERATOR
** Single and double redirection operators
**
** ">" | ">>" | "<"
*/

static t_tok_t	lex_redir_op_type(t_lex_st *st)
{
	t_tok_t		type;

	type = TOK_NONE;
	if (*st->input == '>')
	{
		st->input++;
		if (*st->input == '>')
		{
			type = OP_REDIR_WA;
			st->input++;
		}
		else
			type = OP_REDIR_WR;
	}
	else if (*st->input == '<')
	{
		type = OP_REDIR_RD;
		st->input++;
	}
	return (type);
}

/*
** REDIR
** REDIR_OPERATOR followed by PARAM, separated by IFS
**
** REDIR_OPERATOR IFS PARAM
*/
static t_lex_err	lex_redir(t_tok **tokens, t_lex_st *st)
{
	const t_tok_t	type = lex_redir_op_type(st);
	t_lex_err		status;
	t_tok			*redir;
	t_tok			*redir_data;

	redir_data = NULL;
	if (type == TOK_NONE)
		status = LEX_ENOMATCH;
	else if (lex_ifs(st) != LEX_EOK
	|| (status = lex_param(&redir_data, st, TOK_PARAM)) == LEX_ENOMATCH)
		status = LEX_ESYNTAX;
	else if (status == LEX_EOK)
	{
		if ((redir = token_new(redir_data, type)))
			token_add_back(tokens, redir);
		else
		{
			token_clr(&redir_data);
			status = LEX_EALLOC;
		}
	}
	return (status);
}

/*
** HEREDOC
** HEREDOC_OPERATOR followed by PARAM
**
** "<<" PARAM
*/
static t_lex_err	lex_heredoc(t_tok **tokens, t_lex_st *st)
{
	if (ft_strncmp(st->input, "<<", 2))
		return (LEX_ENOMATCH);
//	ft_dprintf(2, "[LEX][  CMD][INLINE][HEREDOC] Input: '%s'\n", st->input);
	return (lex_param(tokens, st, OP_HEREDOC));
}

/*
** INLINE
** REDIR or HEREDOC
**
** REDIR | HEREDOC
*/
t_lex_err		lex_inline(t_tok **tokens, t_lex_st *st)
{
	t_lex_err		status;

	if ((status = lex_redir(tokens, st)) == LEX_ENOMATCH) // alocates a token redir
		status = lex_heredoc(tokens, st); // alocates a token heredoc
	return (status);
}
