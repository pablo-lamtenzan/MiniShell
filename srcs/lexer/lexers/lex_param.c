#include <lexer/lexer.h>

/*
** PARAM_SIMPLE
** chars that aren't special
**
** ( char - [">|&;'\"\\"] )*
*/
static t_lex_err	lex_param_simple(t_tok **tokens, t_lex_st *st, t_tok_t type)
{
	t_tok		*param;
	const char	*start;

	start = st->input;
//	ft_dprintf(2, "[LEX][PARAM][SIMPLE] Input: '%s'\n", st->input);
	while (*st->input && !is_special(*st->input))
		st->input++;
	if (st->input == start)
		return (LEX_ENOMATCH);
	if (!(param = token_strndup(start, st->input - start, TOK_SIMPLE | type))) // type no quotes
		return (LEX_EALLOC);
	token_add_back(tokens, param);
	return (LEX_EOK);
}

/*
** PARAM
** Multiple PARAM_QUOTEDs or PARAM_SIMPLEs.
**
** (PARAM_QUOTED | PARAM_SIMPLE)+
*/
t_lex_err		lex_param(t_tok **tokens, t_lex_st *st, t_tok_t type) // handles multiple params in a cmd token (cmd.data)
{
	t_lex_err	status;
	t_tok		*param;
	t_tok		*param_data;

	param_data = NULL;
	status = LEX_EOK;
	while (status == LEX_EOK)
		if ((status = lex_param_quoted(&param_data, st, type)) == LEX_ENOMATCH)
			status = lex_param_simple(&param_data, st, type); // allocates a param and set no flag
	if (param_data && status == LEX_ENOMATCH)
	{
		if ((param = token_new(param_data, type)))
		{
			token_add_back(tokens, param);
			status = LEX_EOK;
		}
		else
			status = LEX_EALLOC;
	}
	if (status != LEX_EOK)
		token_clr(&param_data);
	return (status);
}
