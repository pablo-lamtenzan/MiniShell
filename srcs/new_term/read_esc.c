#include "term.h"

/* static t_term_err	term_read_shift(t_term *term, char c)
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
} */

/* static t_term_err	term_read_alt_esc(t_term *term)
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
} */

/*
**	Special key-strokes preceded by ANSI escape.
*/
t_term_err	term_read_esc(t_term *term)
{
	ssize_t	read_st;
	char	c;
	int		repeat;
	int		digits;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == TERM_ESC || c == '\0') // -> esc
		return (TERM_EOK);
	if (c == TERM_CSI)
		return (term_read_csi(term));
	digits = 1;
	repeat = 0;
	while (ft_isdigit(c) && ++digits < 8)
	{
		repeat = (10 * repeat) + (c - '0');
		term_clear_line(term);
		ft_dprintf(STDERR_FILENO, "(arg: %d) ", repeat);
		if ((read_st = read(0, &c, 1)) != 1)
			return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	}
	if (digits == 8)
		term_clear_line(term);
	//ft_dprintf(2, "[PROMPT][ESC][REPEAT] %d\n", repeat);
	return (TERM_EOK);
}
