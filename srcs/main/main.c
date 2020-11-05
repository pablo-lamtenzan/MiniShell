/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 23:28:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/05 04:37:18 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int		exec(t_tok* tokens, t_term* term) // not static for debug
{
    int		status; // to norme
	t_tok*	exec_tokens;
	t_bst*	root;
	int		conditionals; // to norme
	int		parentheses_nb; // to norme

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
		token_clr(&exec_tokens);
		exec_tokens = NULL;
	}
	return (true);
}


int			main(int ac, const char** av, const char** envp)
{	
    return (false && term_prompt(ac, av, envp, &exec));
}