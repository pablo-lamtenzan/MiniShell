/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 19:29:44 by chamada           #+#    #+#             */
/*   Updated: 2020/10/04 20:29:20 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <operator.h>

t_operator		*operator_new(t_operator_t type)
{
	t_operator	*new;

	if (!(new = malloc(sizeof(*new))))
		return (NULL);
	new->type = type;
	new->next = NULL;
	return (new);
}


t_operator *operator_add(t_operator **operators, t_operator *new)
{
	t_operator	*curr;

	if (!new)
		return (NULL);
	new->next = NULL;
	if (*operators)
	{
		curr = *operators;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	else
		*operators = new;
	return (new);
}

t_operator  *operator_clear(t_operator **operators)
{
	t_operator	*curr;

    if (*operators)
    {
        while ((curr = *operators))
        {
            *operators = curr->next;
            free(curr);
        }
    }
	return (NULL);
}
