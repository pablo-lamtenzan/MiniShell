#include "term.h"

/*
**	Special key-strokes preceded by ANSI escape.
*/
t_term_err			term_read_esc(t_term *term)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == TERM_ESC || c == '\0') // -> esc
		return (TERM_EOK);
	if (c == TERM_CSI)
		return (term_read_csi(term));
	if (c == 'b')
		return (cursor_prev_word(term));
	if (c == 'f')
		return (cursor_next_word(term));
	return (TERM_EOK);
}
