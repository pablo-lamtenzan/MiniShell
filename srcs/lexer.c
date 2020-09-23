/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 18:13:09 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/23 17:44:22 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdbool.h>

t_token		*token_clear(t_token **tokens)
{
	t_token	*next;

	while (*tokens)
	{
		next = (*tokens)->next;
		free((*tokens)->data);
		free(*tokens);
		*tokens = next;
	}
	return (*tokens);
}

bool		lexer_tokenize(char **input, t_token **cmds,
		t_token **operators, bool *semicolon)
{
	/* TO DO */
	/* This fuction has to increment the input if one semicolon is founded
		to 1 byte after the semicolon.
		Have to fill "cmds" with everything that isn't an operator.
		Have to fill "operators" with all operator execpt semicolon
		Have to update "semicolon" to false if no semicolon is founded.
		Returns false in error case, true on success.
	*/
	return (true);
}
