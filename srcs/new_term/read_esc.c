#include "term.h"

static t_term_err	term_read_shift(t_term *term, char c)
{
	const t_keybind	bindings[] = {
		{term->caps.key.left[2], &select_left},
		{term->caps.key.right[2], &select_right}
	};
	size_t			i;

	i = 0;
	while (i < sizeof(bindings) / sizeof(*bindings) && c != bindings[i].key)
		i++;
	if (i < sizeof(bindings) / sizeof(*bindings))
		return (bindings[i].action(term));
	return (TERM_EOK);
}

static t_term_err	term_read_alt_esc(t_term *term)
{
	ssize_t			read_st;
	char			c[3];

	if ((read_st = read(0, c, 3)) != 3)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if (c[1] == TERM_SHIFT)
		return (term_read_shift(term, c[2]));
	else if (c[1] == '5')
		ft_dprintf(2, "[PROMPT][ESC][ALT] ctrl + %c\n", c[2]);
	return (TERM_EOK);
}

static t_term_err	term_read_arrow(t_term *term, char c)
{
	const	t_keybind bindings[] = {
		{term->caps.key.up[2], &term_prev_line},
		{term->caps.key.down[2], &term_next_line},
		{term->caps.key.left[2], &cursor_l},
		{term->caps.key.right[2], &cursor_r}
	};
	size_t					i;

	i = 0;
	while (i < sizeof(bindings) / sizeof(*bindings) && c != bindings[i].key)
		i++;
	if (i < sizeof(bindings) / sizeof(*bindings))
		return (bindings[i].action(term));
	return (TERM_EOK);
}


/*
**	Special key-strokes preceded by ASCII escape.
*/
t_term_err	term_read_esc(t_term *term)
{
	ssize_t	read_st;
	char	c[2];

	if ((read_st = read(0, c, 2)) != 2)
		return ((read_st == 0) ? TERM_EEOF : TERM_EREAD);
	if (c[1] == TERM_ALTESC)
		return (term_read_alt_esc(term));
	/* if (status & TERM_SELECT)
	{
		status &= ~TERM_SELECT;
		select_clear(t);
	} */
	select_clear(term);
	term_read_arrow(term, c[1]);
	return (TERM_EOK);
}
