/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/11 00:39:48 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

static t_lex_err	cmd_new(t_tok **tokens,
	t_tok *inline_operators, t_tok *data)
{
	t_lex_err		status;
	t_tok			*cmd;

	status = LEX_EOK;
	if ((cmd = token_new(data, TOK_CMD)))
	{
		cmd->next = inline_operators;
		token_add_back(tokens, cmd);
	}
	else
		status = LEX_EALLOC;
	return (status);
}

/*
** CMD_SIMPLE
** PARAMs or INLINEs separated by IFS
**
** ( IFS ( PARAM | INLINE ) ) *
** TODO: Handle st->wait
*/
static t_lex_err	lex_cmd_simple(t_tok **tokens, t_lex_st *st)
{
	t_lex_err		status;
	t_tok			*inline_operators;
	t_tok			*cmd_data;

	inline_operators = NULL;
	cmd_data = NULL;
	status = LEX_EOK;
	while (status == LEX_EOK && (status = lex_ifs(st)) == LEX_EOK) // skip spaces
		if ((status = lex_inline(&inline_operators, st)) == LEX_ENOMATCH) // handles redir tokens and increments input
			status = lex_param(&cmd_data, st, TOK_PARAM); //handles tokens params
	if ((inline_operators || cmd_data)
	&& (status = cmd_new(tokens, inline_operators, cmd_data)) != LEX_EOK)
	{
		token_clr(&inline_operators);
		token_clr(&cmd_data);
	}
	return (status);
}

/*
** OPERATOR
** PIPE, OR or AND operator
**
** "|" | "||" | "&&"
*/
static t_lex_err	lex_operator(t_tok **tokens, t_lex_st *st)
{
	t_tok_t		type;
	t_tok		*operator;

	if (*st->input == '|')
	{
		st->input++;
		if (*st->input == '|')
		{
			type = OP_OR;
			st->input++;
		}
		else
			type = OP_PIPE;
	}
	else if (!ft_strncmp(st->input, "&&", 2))
	{
		type = OP_AND;
		st->input += 2;
	}
	else
		return (LEX_ENOMATCH);
	if (!(operator = token_new(NULL, type)))
		return (LEX_EALLOC);
	token_add_back(tokens, operator);
	return (LEX_EOK);
}

/*
** CMD
** SUBSHELL or CMD_SIMPLE
**
** SUBSHELL | CMD_SIMPLE
*/
t_lex_err			lex_cmd(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;

/* 	if (st->wait & TOK_SCOPE_OUT)
		return (lex_subshell(st));
	if (st->wait & TOK_CMD)
		return (lex_cmd_simple(st)); */

//	ft_dprintf(2, "[LEX][  CMD] Input: '%s'!\n", st->input);
	if ((status = lex_subshell(tokens, st)) == LEX_ENOMATCH)
		status = lex_cmd_simple(tokens, st);
	return (status);
}

/*
** OPERATION
** OPERATOR followed by IFS and CMD
**
** OPERATOR IFS CMD
*/
t_lex_err			lex_operation(t_tok **tokens, t_lex_st *st)
{
	t_lex_err	status;

//	ft_dprintf(2, "[LEX][   OP] Input: '%s'\n", st->input);
	if ((status = lex_operator(tokens, st)) == LEX_EOK
	&& (status = lex_ifs(st)) == LEX_EOK)
		status = lex_cmd(tokens, st);
	return (status);
}
