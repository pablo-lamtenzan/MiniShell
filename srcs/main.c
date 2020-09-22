/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 17:33:34 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/22 18:31:13 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <minishell.h>

char		*matrix_to_string(char **matrix)
{
	// TO DO
	return ((char *)0);
}

int			routine(char *input, t_data *data) // need to add the global struct
{
	t_token	*comands;
	t_token	*operators;
	t_bst	*bst;
	bool	semicolon;

	semicolon = true;
	/* The steps for execute a comand are lexer, bst build, execution and
		free. If there a semicolon we just have to repeat the process until
		there are semicolons
	*/
	while (semicolon)
	{
		if (*input == '\0'
				|| !lexer_tokenize(&input, &comands, &operators, &semicolon)
				|| !(bst = build_bst(operators, comands)))
			return (false);
		execute_bst(bst, data);
		free_all();
	}
	return (true);
}

int			main(int ac, char **argv, char **envp)
{
	t_data	*data;

	if (!(data = malloc(sizeof(t_data))))
		return (false);
	data->ac = ac;
	data->argv = argv;
	data->envp = envp;
	data->pid = 0;
	return (routine(matrix_to_string(argv), data));
}