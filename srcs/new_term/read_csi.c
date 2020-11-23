#include "term.h"

t_term_err	term_read_mod_none(t_term *term)
{
	ssize_t	read_st;
	char	c;

	(void)term;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';') // TODO: Handle unknown
		return (term_read_csi(term));
	ft_dprintf(2, "[PROMPT][ESC][NONE ] %hu\n", c);
	return (TERM_EOK);
}

t_term_err	term_read_mod_shift(t_term *term)
{
	const	t_keybind bindings[] = {
		{term->caps.key.left[2], &select_left},
		{term->caps.key.right[2], &select_right}
	};
	ssize_t	read_st;
	size_t	i;
	char	c;
	
	(void)term;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	i = 0;
	while (i < sizeof(bindings) / sizeof(*bindings) && c != bindings[i].key)
		i++;
	if (i < sizeof(bindings) / sizeof(*bindings))
		return (bindings[i].action(term));
	return (TERM_EOK);
}

/* 
t_term_err	term_read_mod_alt(t_term *term)
{
	ssize_t	read_st;
	char	c;

	(void)term;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';') // TODO: Handle unknown
	{
		ft_dprintf(2, "[PROMPT][ESC][ALT  ] SEQUENCE!\n");
		return (term_read_csi(term));
	}
	//ft_dprintf(2, "[PROMPT][ESC][ALT  ] %hu\n", c);
	return (TERM_EOK);
} */

/* t_term_err	term_read_mod_ctrl(t_term *term)
{
	(void)term;
	ft_dprintf(2, "[PROMPT][ESC][CTRL ] \n");
	return (TERM_EOK);
}
 */
/* t_term_err	term_read_mod_meta(t_term *term)
{
	ssize_t	read_st;
	char	c;

	(void)term;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';') // TODO: Handle unknown
	{
		ft_dprintf(2, "[PROMPT][ESC][META ] SEQUENCE!\n");
		return (term_read_csi(term));
	}
	ft_dprintf(2, "[PROMPT][ESC][META ] %hu\n", c);
	return (TERM_EOK);
} */

t_term_err	term_read_csi(t_term *term)
{
	t_term_action	mod_handlers[] = {
		&term_read_mod_none,
		&term_read_mod_shift,
//		&term_read_mod_alt,
//		&term_read_mod_ctrl,
//		&term_read_mod_meta // TODO: Apparently this is unhandled by bash
	};
	const	t_keybind bindings[] = {
		{'H', &cursor_start_line}, // TODO: Get in terminfo or remove caps.key
		{'F', &cursor_end_line},
		{term->caps.key.up[2], &term_prev_line},
		{term->caps.key.down[2], &term_next_line},
		{term->caps.key.left[2], &cursor_l},
		{term->caps.key.right[2], &cursor_r}
	};
	(void) term;
	ssize_t			read_st;
	size_t			i;
	char			c;
	unsigned char	mod;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == '\0') // -> Alt-[
		ft_dprintf(2, "[PROMPT][ESC] Alt + [\n");
	if ((mod = c - '1') < sizeof(mod_handlers) / sizeof(*mod_handlers))
		return (mod_handlers[mod](term));
	else
	{
		select_clear(term);
		i = 0;
		while (i < sizeof(bindings) / sizeof(*bindings) && c != bindings[i].key)
			i++;
		if (i < sizeof(bindings) / sizeof(*bindings))
			return (bindings[i].action(term));
	}
	return (TERM_EOK);
}
