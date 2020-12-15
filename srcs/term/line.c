/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 19:30:00 by chamada           #+#    #+#             */
/*   Updated: 2020/09/26 15:15:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/line.h>

t_line	*line_new(size_t size)
{
	t_line	*line;

	if (!(line = ft_calloc(1, sizeof(*line))))
		return (NULL);
	if (size)
	{
		line->size = size;
		if (!(line->data = malloc(sizeof(*line->data) * line->size)))
		{
			free(line);
			return (NULL);
		}
		*line->data = '\0';
	}
	return (line);
}

void	line_add_back(t_line **line_list, t_line *line)
{
	t_line	*current;

	if (line)
	{
		if (*line_list)
		{
			current = *line_list;
			while (current->next)
				current = current->next;
			current->next = line;
		}
		else
			*line_list = line;
	}
}

size_t	line_count(t_line *line_list)
{
	size_t	count;

	count = 0;
	while (line_list)
	{
		line_list = line_list->next;
		count++;
	}
	return (count);
}

void	*line_clear(t_line **line)
{
	if (*line)
	{
		free((*line)->data);
		free(*line);
		*line = NULL;
	}
	return (NULL);
}

void	lines_clear(t_line **line_list)
{
	t_line	*curr;

	while ((curr = *line_list))
	{
		*line_list = (*line_list)->next;
		free(curr->data);
		free(curr);
	}
}
