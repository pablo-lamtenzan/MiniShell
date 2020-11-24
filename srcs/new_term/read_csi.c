#include "term.h"

t_term_err	term_read_mod_none(t_term *term)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';') // TODO: Handle unknown
	{
		//ft_dprintf(2, "[PROMPT][ESC][NONE ] Reentering CSI!\n");
		return (term_read_csi(term));
	}
	ft_dprintf(2, "[PROMPT][ESC][NONE ] %hu\n", c);
	return (TERM_EOK);
}

t_term_err	term_read_mod_shift(t_term *term)
{
	const	t_keybind keys[] = {
		{term->caps.key.left[2], &select_left},
		{term->caps.key.right[2], &select_right}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action(term));
	return (TERM_EOK);
}

t_term_err	term_read_csi(t_term *term)
{
	const t_keybind	mods[] = {
		{'1', &term_read_mod_none},
		{'2', &term_read_mod_shift}
	};
	const t_keybind	keys[] = {
		{'H', &cursor_start_line}, // TODO: Get in terminfo or remove caps.key
		{'F', &cursor_end_line},
		{term->caps.key.up[2], &term_prev_line},
		{term->caps.key.down[2], &term_next_line},
		{term->caps.key.left[2], &cursor_l},
		{term->caps.key.right[2], &cursor_r}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == '\0') // -> Alt-[
		ft_dprintf(2, "[PROMPT][ESC] Alt + [\n");
	if ((action = keybind_get(mods, sizeof(mods) / sizeof(*mods), c)))
		return (action(term));
	select_clear(term);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action(term));
	return (TERM_EOK);
}
