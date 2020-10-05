/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:33:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/05 01:32:37 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

const char *operator_str(t_operator_t type)
{
	if (type == NONE)
		return ("NONE");
	if (type & PIPE)
		return ("PIPE");
	if (type & REDIR_GR)
		return ("REDIR_GR");
	if (type & REDIR_LE)
		return ("REDIR_LE");
	if (type & SEMICOLON)
		return ("SEMICOLON");
	return ("UNKNOWN!");
}

void cmd_print(t_cmd *commands, t_operator *operators)
{
	int	i;

	i = 0;
	while (operators)
	{
		ft_dprintf(2, "[exec][operator][%s] ", operator_str(operators->type));
		while (commands)
		{
			while (i < commands->ac)
				ft_dprintf(2, "%s ", commands->av[i++]);
			commands = commands->next;
		}
		ft_dprintf(2, "\n");
		operators = operators->next;
	}
}

int			exec(const char *input, t_term *term) // need to add the global struct
{
	t_cmd		*commands;
	t_operator	*operators;
	t_bst		*bst;
	int			status;

	/* The steps to execute a comand are lexer, bst build, execution and
		free. If there a semicolon we just have to repeat the process until
		there are no semicolons left
	*/
	while ((status = lexer_tokenize(&input, &commands, &operators)) > 0)
	{
		ft_dprintf(2, "[exec][input] %s\n", input);
		ft_dprintf(2, "[exec][status] %d\n", status);
		cmd_print(commands, operators);
		if (!(bst = build_bst(operators, commands)))
			return (false);
		operator_clear(&operators);
		cmd_clear(&commands);
		/* and comands will be freed while i call free_four_ptrs_and_bst later in execute_bts */
		//execute_bst(bst, term);
		(void) term;
		
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
