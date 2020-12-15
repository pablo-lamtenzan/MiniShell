/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_csi_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/09 10:51:59 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/keybinds.h>

t_term_err	term_read_mod_none(void)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if (c == ';')
		return (term_read_csi());
	return (TERM_EOK);
}

t_term_err	term_read_mod_shift(void)
{
	ssize_t						read_st;
	char						c;
	t_term_action				action;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if ((action = keybind_get(g_keybinds.shifts,
		sizeof(g_keybinds.shifts) / sizeof(*g_keybinds.shifts), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_read_mod_alt(void)
{
	ssize_t					read_st;
	char					c;
	t_term_action			action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if ((action = keybind_get(g_keybinds.alts,
		sizeof(g_keybinds.alts) / sizeof(*g_keybinds.alts), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_read_del(void)
{
	t_term_err	status;
	ssize_t		read_st;
	char		c;

	status = TERM_EOK;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		status = (read_st == 0) ? TERM_EEOF : TERM_EREAD;
	else if (c == '~' && g_term.caps.index != g_term.line->len)
		status = term_line_del(1);
	return (status);
}

t_term_err	term_read_csi(void)
{
	ssize_t					read_st;
	char					c;
	t_term_action			action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if (c == '\0')
		ft_dprintf(2, "[PROMPT][ESC] Alt + [\n");
	if ((action = keybind_get(g_keybinds.mods,
		sizeof(g_keybinds.mods) / sizeof(*g_keybinds.mods), c)))
		return (action());
	select_clear();
	if ((action = keybind_get(g_keybinds.csis,
		sizeof(g_keybinds.csis) / sizeof(*g_keybinds.csis), c)))
		return (action());
	return (TERM_EOK);
}
