/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 18:13:09 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/28 09:50:46 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

static bool			is_operator(char x)
{
	return (x == '|' || x == '>' || x == '<' || x == ';');
}

static bool			correct_input(char** input)
{
	char*			cp = *input;
	char*			c1;
	
	//while (!is_operator(*cp))
	//	cp++;
	if (*cp == '|')
	{
		c1 = cp;
		cp += *(cp + 1) ? 1 : 0;
		while(!is_operator(*cp))
			cp++;
		ft_dprintf(2, "next op is [%c]\n", *cp);
		if (*cp == '<')
			(*input)[c1 - *input] = ';';				
	}
	else if (*cp == '>')
	{
		c1 = cp;
		cp += *(cp + 1) ? 1 : 0;
		while(!is_operator(*cp))
			cp++;
		if (*cp == '|')
			(*input)[cp - *input] = ';';				
	}
	ft_dprintf(2, "cp is >>> [%c]\n", *cp);
	ft_dprintf(2, "input is >>> [%c]\n", **input);
	return (true);
}
// lex_type: Get the type of the current token
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
			return ((status & ~TOKEN) | SEMICOL);
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

// lex_operator: Get the type of the current operator
static t_operator	*lex_operator(const char **txt)
{
	t_operator		*new;
	const int		pos = ft_strpos(OPERATORS, *(*txt)++);

	new = (pos >= 0) ? malloc(sizeof(*new)) : NULL;
	if (new)
	{
		ft_dprintf(2, "[lexer][operator] %c\n", OPERATORS[pos]);
		if ((1 << pos) == REDIR_GR && **txt == '>')
			new->type = REDIR_DG;
		else
			new->type = (1 << pos);
		new->next = NULL;
	}
	return (new);
}

static bool			parse_cmd(t_cmd **cmds, int *status,
	char **input)
{
	const char	*start;
	t_token		*tokens;

	tokens = NULL;
	while (**input && !(*status & (SEMICOL | OP)))
	{
		*status = TOKEN;
		while (ft_isspace(**input))
			(*input)++;
		start = *input;
		while (**input && correct_input(input) && ((*status = lex_type(*status, **input)) & TOKEN))
			(*input)++;
		if (*input != start)
		{
			char *token_data = ft_substr(start, 0, *input - start);
			ft_dprintf(2, "[lexer][token] '%s'\n", token_data);
			if (!token_add(&tokens, token_new(token_data, TOKEN)))
				return (token_clear(&tokens, true));
		}
	}
	return (!tokens || cmd_add(cmds, tokens_export(tokens)));
}

static int			parse_operation(char **input, t_cmd **cmds,
	t_operator **operators)
{
	int				status;

	status = TOKEN;
	if (!parse_cmd(cmds, &status, input))
	{
		ft_dprintf(2, "Error: get_next_cmd returned false!\n");
		cmd_clear(cmds);
		return (ERROR);
	}
	ft_dprintf(2, "[lexer][status] %d\n", status);

	if (status & OP)
	{
		if (!operator_add(operators, lex_operator((const char**)input)))
			return (ERROR);
		//correct_status(&status, operators, (char**)input);
	}
	else if (*cmds && !*operators && !(*operators = operator_new(NONE)))
		return (ERROR);
	if (**input)
		(*input)++;
	return (status);
}

int					lexer_tokenize(char **input, t_cmd **cmds,
	t_operator **operators)
{
	int	status;

	*cmds = NULL;
	*operators = NULL;
	status = EMPTY;
	while (**input && !(status & SEMICOL))
	{
		ft_dprintf(2, "[lexer][input] '%s'\n", *input);
		if ((status = parse_operation(input, cmds, operators)) < 0)
		{
			cmd_clear(cmds);
			operator_clear(operators);
			return (ERROR);
		}
		t_operator* cp = *operators;
		while (cp->next)
			cp = cp->next;
		ft_dprintf(2, "-#########################------------########-> [lexer]OPERATOR IS: [%d] [%p] STATUS IS [%d]\n", cp->type, cp, status);
	}
	if (status == EMPTY || !*operators)
		return (EMPTY);
	return (status & SEMICOL ? CONTINUE : END);
}
