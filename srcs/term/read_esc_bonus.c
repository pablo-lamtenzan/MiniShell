#include <term/term.h>

/*
**	Special key-strokes preceded by ANSI escape.
*/
t_term_err			term_read_esc(void)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ANSI_ESC || c == '\0') // -> esc
		return (TERM_EOK);
	if (c == ANSI_CSI)
		return (term_read_csi());
	if (c == 'b')
		return (cursor_prev_word());
	if (c == 'f')
		return (cursor_next_word());
	return (TERM_EOK);
}
