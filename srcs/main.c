/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:33:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/06 18:50:28 by chamada          ###   ########.fr       */
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

	while (operators)
	{
		ft_dprintf(2, "[exec][operator][%s] ", operator_str(operators->type));
		while (commands)
		{
			i = 0;
			while (i < commands->ac)
			{
				ft_dprintf(2, "%s", commands->av[i++]);
				if (i == commands->ac)
					ft_dprintf(2, " ");
			}
			if ((commands = commands->next))
				ft_dprintf(2, ", ");
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

	/* Lexer Problems:
		- when there is a semicolon "opeartors" doesn't have an address
		- redirection doesn't goes further than lexer.c (inf loop)
		- pipe doesn't goes further than lexer.c (returns error)

		Builtins problems:
		- echo
		- export
		- unset
		
		Builting seems to work:
		- exit
		- pwd
		- cd
		- env
	*/
	while ((status = lexer_tokenize(&input, &commands, &operators)) > 0)
	{
		ft_dprintf(2, "[exec][operators address] %p\n", operators);
		ft_dprintf(2, "[exec][input] '%s'\n", input);
		ft_dprintf(2, "[exec][status] %d\n", status);
		cmd_print(commands, operators);
		//ft_dprintf(2, "Goes into build_bst\n");
		if (!(bst = build_bst(operators, commands)))
			return (false);
		/*
		ft_dprintf(2, "[bst][address] %p\n", bst);
		ft_dprintf(2, "[bst][operator] %d\n", bst->operator);
		ft_dprintf(2, "[bst][av[0][0]] %s\n", bst->av[0][0]);
		ft_dprintf(2, "[bst][av[0][1]] %s\n", bst->av[0][1]);
		*/
		operator_clear(&operators);
		cmd_clear(&commands);
		/* and comands will be freed while i call free_four_ptrs_and_bst later in execute_bts */
		execute_bst(bst, term);
		ft_dprintf(2, "[exec] Done\n");
	}
	if (status == ERROR)
		return (false);
	return (true);
}

int			main(int ac, const char **av, const char **envp)
{
	int	status;

	status = term_prompt(ac, av, envp, &exec);
	// TODO: Free everything
	return (status);
}
