/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/13 00:34:09 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

/*
**	Get a c-string's graphical character count
*/
size_t		strglen(const char *str)
{
	size_t	len;
	size_t	glen;

	len = 0;
	glen = 0;
	while (str[len])
	{
		if (str[len] == ANSI_ESC && str[++len] == ANSI_CSI)
		{
			while ((str[++len] & 0xF0) == 0x30)
				;
			while ((str[len] & 0xF0) == 0x20)
				len++;
			if ((str[len] & 0xC0) == 0x40)
				len++;
		}
		else if (ft_isprint(str[len++]))
			glen++;
	}
	return (glen);
}

t_term_err	cursor_insert(const char *input, size_t length)
{
	if (caps_insert(&g_term.caps, input, length) == -1)
		return (TERM_EWRITE);
	g_term.caps.index += length;
	g_term.caps.cursor.pos = (t_pos) {
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			% (g_term.caps.width),
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			/ (g_term.caps.width) + g_term.caps.cursor.origin.y,
	};
	if (g_term.caps.cursor.pos.x == 0)
		cursor_d();
	return (TERM_EOK);
}

t_term_err	cursor_write(const char *input, size_t length)
{
	if (write(STDERR_FILENO, input, length) == -1)
		return (TERM_EWRITE);
	g_term.caps.index += length;
	g_term.caps.cursor.pos = (t_pos) {
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			% (g_term.caps.width),
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			/ (g_term.caps.width) + g_term.caps.cursor.origin.y,
	};
	if (g_term.caps.cursor.pos.x == 0)
		cursor_d();
	return (TERM_EOK);
}

t_term_err	term_write(const char *input, size_t length)
{
	size_t		remaining;
	t_pos		pos;
	size_t		index;
	t_term_err	status;

	status = TERM_EOK;
	if (length && (status = cursor_insert(input, length)) == TERM_EOK
	&& (remaining = g_term.line->len - g_term.caps.index)
	&& (g_term.caps.cursor.pos.x == 0
	|| length > remaining
	|| g_term.caps.cursor.pos.x + remaining >= (size_t)g_term.caps.width))
	{
		pos = g_term.caps.cursor.pos;
		index = g_term.line->len - remaining;
		tputs(g_term.caps.modes.insert_end, 1, &putc_err);
		g_term.caps.mode &= ~CAPS_MINS;
		term_clear_eos();
		status = cursor_write(g_term.line->data + index, remaining);
		caps_goto(&g_term.caps, pos);
		tputs(g_term.caps.modes.insert, 1, &putc_err);
		g_term.caps.mode |= CAPS_MINS;
		g_term.caps.index -= remaining;
	}
	return (status);
}

t_term_err	term_origin(const char *input, size_t length)
{
	t_term_err		status;
	const size_t	graphical_len = strglen(input);

	status = TERM_EOK;
	if (length && write(STDERR_FILENO, input, length) == -1)
		status = TERM_EWRITE;
	else if (g_term.has_caps)
	{
		g_term.caps.cursor.origin = (t_pos) {
			graphical_len % g_term.caps.width,
			graphical_len / g_term.caps.width
		};
		g_term.caps.cursor.pos = g_term.caps.cursor.origin;
		if (g_term.caps.cursor.pos.x == 0)
			cursor_d();
	}
	return (status);
}
