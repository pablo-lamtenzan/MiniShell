/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 10:34:35 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:52:58 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static	void	free_node(t_exec *args, t_env **first, t_env **prev,
		t_env **next)
{
	if (*first && *first == *args->env)
		*first = (*first)->next;
	free(*args->env);
	if (*prev)
		(*prev)->next = *next;
}

int		b_unset(t_exec *args)
{
	t_env	*next;
	t_env	*first;
	t_env	*prev;

	if (args->ac == 1)
		return (STD_ERROR);
	while (args->ac-- > 1)
	{
		prev = NULL;
		first = *args->env;
		while (*args->env)
		{
			next = (*args->env)->next;
			if (!ft_strncmp(args->av[args->ac], (*args->env)->key, \
					ft_strlen(args->av[args->ac])))
				free_node(args, &first, &prev, &next);
			else
				prev = *args->env;
			*args->env = next;
		}
		*args->env = first;
	}
	return (SUCCESS);
}
