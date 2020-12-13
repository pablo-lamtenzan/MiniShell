/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_esc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/09 10:51:59 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

/*
**	Special key-strokes preceded by ANSI escape.
*/
t_term_err			term_read_esc(void)
{
	static const t_keybind	keys[] = {
		{ANSI_CSI, &term_read_csi},
		{'b', &cursor_prev_word},
		{'f', &cursor_next_word},
	};
	t_term_action	action;
	ssize_t			read_st;
	char			c;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ANSI_ESC || c == '\0') // -> esc
		return (TERM_EOK);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}
