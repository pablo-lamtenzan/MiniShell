/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 09:51:54 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <separators.h>
#include <builtins.h>

// TODO: child with signal returns t->st = 128 + sig_n DONE 
// TODO: Local path_get (start with '/' or '.')
// TODO: Nb process printed in print_signals (for inperrumpt signals) into pipe
// TODO: Change builtins and child error codes by defines

/*
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
*/

static int exec(t_tok* tokens, t_term* term)
{
	int		flags[3];
	t_tok*	exec_tokens;
	t_bst*	root;

	ft_bzero(flags, sizeof(flags));
	while ((exec_tokens = handle_separators(&tokens, &flags[STATUS], &flags[PARETHESES_NB])))
	{
//		token_print(exec_tokens, "EXE");
		if (handle_conditionals(&term, flags[STATUS], &flags[CONDITIONALS], flags[PARETHESES_NB]))
		{
			if (!execute_bst(root = bst(exec_tokens), term))
			{
				free_bst(root);
				ft_exit(NULL, term);
			}
			free_bst(root);
		}
	}
	return (0);
}


int		main(int ac, const char** av, const char** envp)
{	
    return (term_prompt(ac, av, envp, &exec));
}
