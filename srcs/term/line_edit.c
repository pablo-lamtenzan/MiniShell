/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 19:30:03 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 00:17:46 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/line.h>

bool	line_resize(t_line *line, size_t new_size)
{
	char	*new;

	if (new_size <= line->size)
		return (true);
	while (new_size > line->size)
		line->size *= 2;
	if (!(new = malloc(sizeof(*new) * line->size)))
		return (false);
	ft_strlcpy(new, line->data, line->size);
	free(line->data);
	line->data = new;
	return (true);
}

bool	line_insert(t_line *line, size_t at, const char *str, size_t n)
{
	size_t	new_len;

	if (at > line->len)
		return (false);
	new_len = line->len + n;
	if (!line_resize(line, new_len + 1))
		return (false);
	ft_memmove(line->data + at + n, line->data + at, line->len - at);
	ft_memcpy(line->data + at, str, n);
	line->len = new_len;
	line->data[line->len] = '\0';
	return (true);
}

bool	line_erase(t_line *line, size_t at, size_t n)
{
	if (!line || line->len - at < n)
		return (false);
	if (at == line->len)
		line->len -= n;
	else
	{
		ft_memmove(line->data + at, line->data + at + n, line->len - at - n);
		line->len -= n;
	}
	line->data[line->len] = '\0';
	return (true);
}

t_line	*line_dup(t_line *line)
{
	t_line	*new;

	if (!line || !(new = malloc(sizeof(*new))))
		return (NULL);
	new->size = line->size;
	if (!(new->data = malloc(sizeof(*new->data) * new->size)))
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(new->data, line->data, line->len);
	new->len = line->len;
	new->data[new->len] = '\0';
	return (new);
}
