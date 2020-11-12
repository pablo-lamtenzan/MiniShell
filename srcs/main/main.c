/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 23:28:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 06:22:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <separators.h>

void	token_print(t_tok *tokens, const char *prefix)
{
	t_tok *curr;

	curr = tokens;
	while (curr)
	{
			if (curr->type & TOK_CMD)
					token_print(curr->data, "CMD");
			else
					ft_dprintf(2, "[%s][%5d] '%s'\n", prefix, curr->type, curr->data);
			curr = curr->next;
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
		if (handle_conditionals(&term, flags[STATUS], &flags[CODITIONALS], flags[PARETHESES_NB]))
		{
			execute_bst(root = bst(exec_tokens), term);
			free_bst(root);
		}
		exec_tokens = NULL;
	}
	return (true);
}


int			main(int ac, const char** av, const char** envp)
{	
    return (term_prompt(ac, av, envp, &exec));
}
