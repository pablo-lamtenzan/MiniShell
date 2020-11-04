/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst_fill.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:44:01 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 01:14:22 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>

t_tok*		find_next_operator(t_tok* start, t_tok_t type)
{
	while (start->next && !(start->type & type))
		start = start->next ? start->next : start;
	return (start);
}

t_tok*		find_last_operator(t_tok* start, t_tok* end)
{
	t_tok*	last;

	if (start->next != end)
	{
		last = start;
		while (last->next && last->next != end)
			last = last->next;
		if (last->type & CMD)
		{
			while (start->next && start->next != last)
				start = start->next;
		}
		else
			start = last;
	}
	return (start);
}

t_bst*		new_node(void* a, void* b, t_tok_t type)
{
    t_bst* 	new_node;

    if (!(new_node = malloc(sizeof(t_bst))))
        return (NULL);
    *new_node = (t_bst){.a=a, .b=b, .type=type};
    return (new_node);
}

t_tok*		is_pipe_cmd(t_tok* start)
{
	t_tok*	cp;

	cp = start;
	while (cp->next)
	{
		if (cp->type & PIPE)
			return (NULL);
		cp = cp->next;
	}
	return (cp);
}