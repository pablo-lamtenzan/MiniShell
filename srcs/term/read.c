#include <term/term.h>

/*
**	Read and parse a terminal's input.
*/
int		term_read()
{
	int			read_st;

	free(g_term.line->data);
	g_term.line->data = NULL;
	g_term.line->len = 0;
	g_term.line->size = 0;
	if ((read_st = get_next_line(STDIN_FILENO, &g_term.line->data)) == -1)
		return (TERM_EREAD);
	g_term.line->len = ft_strlen(g_term.line->data);
	g_term.line->size = g_term.line->len + 1;
	if (read_st == 1 || (read_st == 0 && g_term.line->len != 0))
		return (TERM_ENL);
	return (TERM_EEOF);
}
