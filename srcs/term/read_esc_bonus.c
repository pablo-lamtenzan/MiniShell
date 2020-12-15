/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_esc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/09 10:51:59 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/keybinds.h>

/*
**	Special key-strokes preceded by ANSI escape.
*/

t_term_err	term_read_esc(void)
{
	t_term_action			action;
	ssize_t					read_st;
	char					c;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if (c == ANSI_ESC || c == '\0')
		return (TERM_EOK);
	if ((action = keybind_get(g_keybinds.escs,
		sizeof(g_keybinds.escs) / sizeof(*g_keybinds.escs), c)))
		return (action());
	return (TERM_EOK);
}
