/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:33:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/04 20:28:46 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int			exec(const char *input, t_term *term) // need to add the global struct
{
	t_cmd		*command;
	t_operator	*operators;
	t_bst		*bst;
	int			status;

	/* The steps for execute a comand are lexer, bst build, execution and
		free. If there a semicolon we just have to repeat the process until
		there are semicolons
	*/
	while ((status = lexer_tokenize(&input, &command, &operators)) > 0)
	{
		if (!(bst = build_bst(operators, command)))
			return (false);
		operator_clear(&operators);
		/* and comands will be freed while i call free_four_ptrs_and_bst later in execute_bts */
		execute_bst(bst, term);
		
	}
	if (status == ERROR)
		return (false);
	return (true);
}

int			main(int ac, const char **av, const char **envp)
{
	int	status;

	status = term_prompt(ac, av, envp, exec);
	// TODO: Free everything
	return (status);
}
