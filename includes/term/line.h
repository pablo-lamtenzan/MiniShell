/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 12:29:43 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 01:31:32 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

# include <stdlib.h>
# include <stdbool.h>

# include <libft.h>

typedef struct	s_line
{
	char			*data;
	size_t			len;
	size_t			size;
	struct s_line	*next;
	struct s_line	*prev;
}				t_line;

/*
**				line.c
*/
t_line			*line_new(size_t size);

void			line_add_back(t_line **line_list, t_line *line);

size_t			line_count(t_line *line_list);

void			*line_clear(t_line **line);
void			lines_clear(t_line **line_list);

/*
**				line_edit.c
*/
t_line			*line_dup(t_line *line);
bool			line_resize(t_line *line, size_t new_size);
bool			line_insert(t_line *line, size_t at, const char *str, size_t n);
bool			line_append(t_line *line, const char *str);
bool			line_erase(t_line *line, size_t at, size_t n);

/*
**				line_put.c
*/
bool			line_puts(t_line *line, const char *str);
bool			line_putc(t_line *line, char c);

#endif
