/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:33:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/18 20:09:57 by pablo            ###   ########.fr       */
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
				if (i < commands->ac)
					ft_dprintf(2, " ");
			}
			if ((commands = commands->next))
				ft_dprintf(2, ", ");
		}
		ft_dprintf(2, "\n");
		operators = operators->next;
	}
}

void		clear_bst(t_bst* bst)
{
	t_bst*	aux;
	int		i;

	int debug = 0;

	while (bst)
	{
		ft_dprintf(2, "[CLEAR BST NODE %d]\n", ++debug);
		aux = bst;
		bst = bst->next;
		i = 0;
		//while (aux->av[0][i++])
		//	free(aux->av[0][i]);
		free(aux->av[0]);
		i = 0;
		//while (aux->av[1][i++])
		//	free(aux->av[1][i]);
		free(aux->av[1]);
		free(aux);
	}
}

int			exec(const char *input, t_term *term) // need to add the global struct
{
	t_cmd		*commands;
	t_operator	*operators;
	t_bst		*bst;
	int			status;

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
		execute_bst(bst, NULL, term);
		clear_bst(bst);
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
