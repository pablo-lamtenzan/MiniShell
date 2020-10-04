/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 18:13:09 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/04 20:53:55 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int      	lex_type(int status, char c)
{
	if (status & B_SLASH)
			return (status & ~B_SLASH);
	if (status & S_QUOTE && c != '\'')
			return (status);
	if (c == '"')
			return (status ^ D_QUOTE);
	if (!(status & D_QUOTE))
	{
		if (c == ';')
			return (status ^ END);
		if (c == '\'')
			return (status ^ S_QUOTE);
		if (ft_isspace(c))
			return (status & ~TOKEN);
		if (c == '\\')
			return (status | B_SLASH);
		if (ft_strchr(OPERATORS, c))
			return ((status & ~TOKEN) | OP);
	}
	return (status);
}

static t_operator	*lex_operator(const char **txt)
{
	t_operator		*new;
	int				pos;

	new = NULL;
	if ((pos = ft_strpos(OPERATORS, *(*txt++))) > 0)
	{
		
		if ((new = malloc(sizeof(*new)))){
			if (pos == REDIR_GR && **txt == '>')
				new->type = REDIR_DG;
			else
				new->type = (1 << pos);
			new->next = NULL;
		}
	}
	return (new);
}


bool					get_next_cmd(t_cmd **cmds, int *status,
	const char **input)
{
	const char	*start;
	t_token		*tokens;

	tokens = NULL;
	*status = TOKEN;
	while (ft_isspace(**input))
		(*input)++;
	start = *input;
	while (**input && ((*status = lex_type(*status, **input)) & TOKEN) == TOKEN)
		(*input)++;
	if (*input != start
	&& !token_add(&tokens, token_new(ft_substr(start, 0, *input - start), TOKEN)))
		return (token_clear(&tokens, true));
	return (cmd_add(cmds, tokens_export(tokens)));
}

int					add_token(const char **input, t_cmd **cmds,
	t_operator **operators)
{
	int				status;

	if (!get_next_cmd(cmds, &status, input))
	{
		cmd_clear(cmds);
		return (ERROR);
	}
	if (status & END)
		return (status);
	if (status & OP && !operator_add(operators, lex_operator(input)))
		return (ERROR);
	return (status);
}

int					lexer_error(t_cmd **cmds, t_operator **operators)
{
	if (cmds)
		cmd_clear(cmds);
	if (operators)
		operator_clear(operators);
	return (ERROR);
}

int					lexer_tokenize(const char **input, t_cmd **cmds,
	t_operator **operators)
{
	int	status;

	*cmds = NULL;
	*operators = NULL;
	ft_dprintf(2, "[lexer][input]: %s", *input);
	if (!*input || !**input)
		return (EMPTY);
	status = TOKEN;
	while (**input)
		if ((status = add_token(input, cmds, operators)) < 0)
			return (lexer_error(cmds, operators));
	if (!*operators && !(*operators = operator_new(NONE)))
		return (lexer_error(cmds, operators));
	return (status & SEMICOL ? CONTINUE : END);
}
