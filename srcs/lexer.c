/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 18:13:09 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/01 22:11:55 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static int      lex_type(int status, char c)
{
	if (status & B_SLASH)
			return (status & ~B_SLASH);
	if (status & S_QUOTE && c != '\'')
			return (status);
	if (c == '"')
			return (status ^ D_QUOTE);
	if (!(status & D_QUOTE))
	{
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

bool			get_next_cmd(t_cmd **cmds, int *status, char **input)
{
	char	*start;
	t_token	*tokens;

	tokens = NULL;
	*status = TOKEN;
	while (ft_isspace(*input))
		input++;
	start = input;
	while (*input && (*status = lex_type(*status, *input)) & TOKEN)
		input++;
	if (input != start
	&& !token_add(tokens, token_new(ft_substr(start, 0, input - start), TOKEN)))
		return (token_clear(tokens, true));
	return (cmd_add(cmds, tokens_export(tokens)));
}

bool			add_token(char **input, t_cmd **cmds, t_token **operators)
{
	int				status;

	if (!get_next_cmd(cmds, &status, input))
		return (cmd_clear(cmds));
	return (status & OP && !token_add(operators, parse_token(&input)));
}

bool			lexer_tokenize(const char *input, t_cmd **cmds,
	t_token **operators, bool *semicolon)
{
	*cmds = NULL;
	*operators = NULL;
	ft_dprintf(2, "[input is %s]\n", input);
	while (*input)
		if (!add_token(&input, cmds, &operators))
			return (token_clear(&operators, true));

	if (!*operators && !(*operators = token_new(ft_strdup(""), NONE))) // type none
		return (token_clear(&operators, true));
	return (true);
}
