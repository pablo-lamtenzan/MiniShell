/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 23:28:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 04:33:24 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

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

int		exec(t_tok* tokens, t_term* term) // not static for debug
{
    int		status; // to norme
	t_tok*	exec_tokens;
	t_bst*	root;
	int		conditionals; // to norme
	int		parentheses_nb; // to norme

	token_print(tokens, "TOK");
	conditionals = NONE;
	status = NONE;
	parentheses_nb = NONE; // join those 3 in a int var_name[3] and bzero sizeof(var_name)
	while ((exec_tokens = handle_separators(&tokens, &status, &parentheses_nb)))
	{
		if (handle_conditionals(&term, status, &conditionals, &parentheses_nb))
		{
			execute_bst(root = bst(exec_tokens), term);
			free_bst(root);
		}
		//token_clr(&exec_tokens);
		exec_tokens = NULL;
	}
	return (0);
}


int		main(int ac, const char** av, const char** envp)
{	
    return (term_prompt(ac, av, envp, &exec));
}
