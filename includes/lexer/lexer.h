/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 03:56:49 by chamada           #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>

# include <lexer/token.h>
# include <env.h>

typedef uint16_t	t_tok_t;

/*
** A character that, when unquoted, separates words
*/
# define LEX_META	" \t\n|&;()<>"

/*
** A character that, when unquoted, quotes following words
*/
# define LEX_QUOT	"'\"\\"

/*
** A character that, when unquoted, cannot be part of a word
** # define LEX_SPEC	LEX_META""LEX_QUOT
*/
# define LEX_SPEC	" \t\n|&;()<>'\"\\"

/*
** LEX_EALLOC: Could not allocate memory!
** LEX_ESYNTAX: Syntax error!
** LEX_ENOMATCH: Incompatible lexer!
** LEX_EWAIT: Waiting for more input!
** LEX_EEND: End of input!
*/
typedef enum		e_lex_err
{
	LEX_EALLOC = -3,
	LEX_ESYNTAX = -2,
	LEX_ENOMATCH = -1,
	LEX_EOK = 0,
	LEX_EEND = 1,
}					t_lex_err;

typedef struct		s_lex_st
{
	const char	*input;
	int			subshell_level;
}					t_lex_st;

/*
** returns true if the character cannot be part of an unquoted word
*/
bool				is_special(char c);

/*
** TOKENS
** TOKENs separated by IFS.
**
** IFS ( TOKEN IFS )*
*/
t_lex_err			lex_tokens(t_tok **tokens, t_lex_st *st);

/*
** SUBSHELL
** TOKENS wrapped in parenthesis.
**
** '(' TOKENS ')'
*/
t_lex_err			lex_subshell(t_tok **tokens, t_lex_st *st);

/*
** CMD
** CMD_SIMPLEs followed by OPERATIONs.
**
** CMD_SIMPLEs OPERATION*
*/
t_lex_err			lex_cmd(t_tok **tokens, t_lex_st *st);

/*
** OPERATION
** OPERATOR followed by CMD
**
** OPERATOR CMD
*/
t_lex_err			lex_operation(t_tok **tokens, t_lex_st *st);

/*
** PARAM
** Multiple PARAM_QUOTEDs or PARAM_SIMPLEs.
**
** (PARAM_QUOTED | PARAM_SIMPLE)*
*/
t_lex_err			lex_param(t_tok **tokens, t_lex_st *st, t_tok_t type);

/*
** PARAM_QUOTED
** PARAM_SQUOTED or PARAM_DQUOTED.
**
** PARAM_SQUOTED | PARAM_DQUOTED
*/
t_lex_err			lex_param_quoted(t_tok **tokens, t_lex_st *st,
	t_tok_t type);

/*
** PARAM_SQUOTED
** chars preceded by backslash or chars wrapped in single quotes, escaped by
** backslash.
**
** '\'' ( char - \' )* '\''
*/
t_lex_err			lex_param_squoted(t_tok **tokens, t_lex_st *st,
	t_tok_t type);

/*
** PARAM_DQUOTED
** chars wrapped in double quotes, escaped by backslash.
**
** '"' ( ( char - ["\"\\"] ) | ( '\\' ["\"\\"] ) )* '"'
*/
t_lex_err			lex_param_dquoted(t_tok **tokens, t_lex_st *st,
	t_tok_t type);

/*
** INLINE
** REDIR or HEREDOC
**
** REDIR | HEREDOC
*/
t_lex_err			lex_inline(t_tok **tokens, t_lex_st *st);

/*
** IFS
** Whitespaces
**
** ( ' ' | [\t .. \r] )*
*/
t_lex_err			lex_ifs(t_lex_st *st);

#endif
