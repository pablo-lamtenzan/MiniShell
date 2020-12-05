#include <term/term.h>

t_term_err	term_read_mod_none(void)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';') // TODO: Handle unknown
	{
		//ft_dprintf(2, "[PROMPT][ESC][NONE ] Reentering CSI!\n");
		return (term_read_csi());
	}
	ft_dprintf(2, "[PROMPT][ESC][NONE ] %hhu\n", c);
	return (TERM_EOK);
}

t_term_err	term_read_mod_shift(void)
{
	const	t_keybind	keys[] = {
		{g_term.caps.keys.left[2], &select_left},
		{g_term.caps.keys.right[2], &select_right}
	};
	ssize_t				read_st;
	char				c;
	t_term_action		action;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_read_mod_alt(void)
{
	const t_keybind	keys[] = {
		{'D', select_prev_word},
		{'C', select_next_word}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_del(void)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == '~' && g_term.caps.index != g_term.line->len)
	{
		caps_delete(&g_term.caps, 1);
		line_erase(g_term.line, g_term.caps.index, 1);
	}
	return (TERM_EOK);
}

t_term_err	term_read_csi(void)
{
	const t_keybind	mods[] = {
		{'1', &term_read_mod_none},
		{'2', &term_read_mod_shift},
		{'4', &term_read_mod_alt}
	};
	const t_keybind	keys[] = {
		{'H', &cursor_start_line}, // TODO: Get in terminfo or remove caps.key
		{'F', &cursor_end_line},
		{g_term.caps.keys.up[2], &term_prev_line},
		{g_term.caps.keys.down[2], &term_next_line},
		{g_term.caps.keys.left[2], &cursor_l},
		{g_term.caps.keys.right[2], &cursor_r},
		{g_term.caps.keys.del[2], &term_del}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == '\0') // -> Alt-[
		ft_dprintf(2, "[PROMPT][ESC] Alt + [\n");
	if ((action = keybind_get(mods, sizeof(mods) / sizeof(*mods), c)))
		return (action());
	select_clear();
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}
