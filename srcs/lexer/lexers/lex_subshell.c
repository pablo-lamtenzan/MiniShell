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
		st->wait |= TOK_SCOPE_OUT;
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

//	ft_dprintf(2, "[LEX][SCOPE][OUT] Input: '%s'\n", st->input);
	if (*st->input == ')')
	{
//		ft_dprintf(2, "[LEX][SCOPE][OUT] MATCH!\n");
		if (st->subshell_level == 0)
			return (LEX_ESYNTAX);
		st->wait &= ~TOK_SEP;
		st->input++;
		st->subshell_level--;
		if (st->subshell_level == 0)
			st->wait &= ~TOK_SCOPE_OUT;
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

//	ft_dprintf(2, "[LEX][SUBSH] Input: '%s'\n", st->input);
	// TODO: Fix scope resume
	if ((status = lex_scope_in(tokens, st)) == LEX_EOK)
	{
//		ft_dprintf(2, "[LEX][SUBSH][ IN] Status: %d, Wait: '%d'\n", status, st->wait);
		if ((status = lex_tokens(tokens, st)) == LEX_EOK
		|| (status == LEX_EWAIT && st->wait & (TOK_SCOPE_OUT | TOK_SEP)))
		{
			status = lex_scope_out(tokens, st);
//			ft_dprintf(2, "[LEX][SUBSH][OUT] Status: %d Wait: '%d'\n", status, st->wait);
		}
//		ft_dprintf(2, "[LEX][SUBSH][END] Status '%d'\n", status);
	}
	return (status);
}

/* MULTINE IMPLEMENTATION */

/*
** NOTES:
** - When the multine is end the history remember to hole line (logical)
** - It seems any level of subshells can be reached in multine format in bash
** - Only seems to work with conditional separators or pipe operator
**
** IMPLEMENTATION IDEA
** - If theres a multiline operator/separator put a flag
** - If after this operator theres something remove the flag
** - If starting a new lexer operation (new line lexed) the flag is on
**		minish must require more input in a new line and block the execution
** - Need to remember all the lexer flags/status of the previous lines
** - So, need just a kind a linked-list who stores the t_lex_st
**		- each operation, a node is created and keeped or deleted if theres no multiline
**		- if there's multiline the nodes are keeped and added like a queue, yeah is that what i need: a queue
**		- just need to use the nodes to execute:
**			- 1) One node means a normal execution (and will be deleted just after send the tokens to the execution engine)
**			- 2) More than one is the same but have to concatenate all the tokens
*/

/* I WILL IMPLEMENT ALL IN THIS FILE FOR THE MOMMENT */

/* First of all we need a mini-lib to create-operate-destroy the queue */

/* Data storage */
typedef struct			s_input_line
{
	t_lex_st			*lexed_lines;
	struct s_input_line	*next;
}						t_input_line;

/* Creation */
t_input_line			*input_line_new(t_lex_st *input)
{
	t_input_line		*input_line;

	if (!(input_line = malloc(sizeof(t_input_line))))
		return (NULL);
	*input_line = (t_input_line){.lexed_lines=input, .next=NULL};
	return (input_line);
}

/* Destruction (destroy all always) */
void					input_line_destroy(t_input_line *head)
{
	t_input_line		*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

/* Adding a node, is bool because is implemented to do: "input_line_push_back(&input_line, input_line_new(st)); */
bool					input_line_push_back(t_input_line **lines, t_input_line *new_line)
{
	t_input_line		*first;

	if (!new_line)
		return (false);
	first = *lines;
	/* At least one input line in the queue */
	if (*lines)
	{
		while ((*lines)->next)
			*lines = (*lines)->next;
		(*lines)->next = new_line;
		*lines = first;
	}
	/* Queue is empty */
	else
		*lines = new_line;
	return (true);
}
