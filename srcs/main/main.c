/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 22:24:24 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <separators.h>

// TODO: child with signal returns t->st = 128 + sig_n
// TODO: Local path_get (start with '/' or '.')

void	token_print(t_tok *tokens, const char *prefix)
{
	ft_dprintf(2, "[%s]\n", prefix);
	while (tokens)
	{
		if (tokens->type & TOK_CMD)
			token_print(tokens->data, "CMD");
		else if (tokens->type == TOK_PARAM)
			token_print(tokens->data, "PRM");
		else
			ft_dprintf(2, "[%s][%5hu] '%s'\n", prefix, tokens->type, tokens->data);
		tokens = tokens->next;
	}
}

static int exec(t_tok* tokens, t_term* term)
{
	int		flags[3];
	t_tok*	exec_tokens;
	t_bst*	root;

	token_print(tokens, "TOK");
	ft_bzero(flags, sizeof(flags));
	while ((exec_tokens = handle_separators(&tokens, &flags[STATUS], &flags[PARETHESES_NB])))
	{
		token_print(exec_tokens, "EXE");
		if (handle_conditionals(&term, flags[STATUS], &flags[CONDITIONALS], flags[PARETHESES_NB]))
		{
			execute_bst(root = bst(exec_tokens), term);
			free_bst(root);
		}
		exec_tokens = NULL;
	}
	return (0);
}


int		main(int ac, const char** av, const char** envp)
{	
    return (term_prompt(ac, av, envp, &exec));
}
