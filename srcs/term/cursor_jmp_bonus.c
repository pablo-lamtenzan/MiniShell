/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_jmp_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/13 00:40:13 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

void		cursor_goto_index(size_t index)
{
	t_pos	pos;

	if (index != g_term.caps.index)
	{
		g_term.caps.index = index;
		index += g_term.caps.cursor.origin.x;
		pos = (t_pos) {
			index % (g_term.caps.width),
			index / (g_term.caps.width) + g_term.caps.cursor.origin.y,
		};
		caps_goto(&g_term.caps, pos);
	}
}

/*
**	Place the cursor at the origin.
**
**	Updates the cursor's position and real position.
*/

t_term_err	cursor_start_line(void)
{
	caps_goto(&g_term.caps, g_term.caps.cursor.origin);
	g_term.caps.index = 0;
	return (TERM_EOK);
}

t_term_err	cursor_end_line(void)
{
	cursor_goto_index(g_term.line->len);
	return (TERM_EOK);
}

t_term_err	cursor_next_word(void)
{
	size_t	i;

	i = g_term.caps.index;
	while (i < g_term.line->len && !ft_isalnum(g_term.line->data[i]))
		i++;
	while (i < g_term.line->len && ft_isalnum(g_term.line->data[i]))
		i++;
	if (i != g_term.caps.index)
		cursor_goto_index(i);
	return (TERM_EOK);
}

t_term_err	cursor_prev_word(void)
{
	size_t	i;

	i = g_term.caps.index;
	while (i > 0 && !ft_isalnum(g_term.line->data[i - 1]))
		i--;
	while (i > 0 && ft_isalnum(g_term.line->data[i - 1]))
		i--;
	if (i != g_term.caps.index)
		cursor_goto_index(i);
	return (TERM_EOK);
}
