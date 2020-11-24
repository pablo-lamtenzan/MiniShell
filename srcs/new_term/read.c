#include "term.h"

/*
**	Control key-strokes.
*/
t_term_err	term_cntrl(t_term *term, char c)
{
	static const t_keybind	keys[] =
	{
		{CINTR, &term_interrupt},
		{CERASE, &term_backspace},
		{CEOF, &term_eof},
		{CSTOP, &term_stop},
		{CSUSP, &term_suspend},
		{'h' - TERM_CNTRL, &term_backspace},
		{'l' - TERM_CNTRL, &term_clear_screen},
		{'j' - TERM_CNTRL, &term_new_line},
		{'a' - TERM_CNTRL, &cursor_start_line},
		{'e' - TERM_CNTRL, &cursor_end_line},
		{'y' - TERM_CNTRL, &clip_paste},
		{'k' - TERM_CNTRL, &clip_cut},
		{'p' - TERM_CNTRL, &term_prev_line},
		{'n' - TERM_CNTRL, &term_next_line}
	};
	t_term_action	action;

	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action(term));
	return (TERM_EOK);
}

/*
**	Read and parse a terminal's input.
*/
int		term_read(t_term *term)
{
	t_term_err	status;
	int			read_st;

	status = TERM_EOK;
	free(term->line->data);
	term->line->data = NULL;
	term->line->len = 0;
	term->line->size = 0;
	if ((read_st = get_next_line(STDIN_FILENO, &term->line->data)) == -1)
		return (TERM_EREAD);
	term->line->len = ft_strlen(term->line->data);
	term->line->size = term->line->len + 1;
	if (read_st == 1 || (read_st == 0 && term->line->len != 0))
	{
		ft_dprintf(2, "[PROMPT][GNL][%3lu] '%s'\n", term->line->len, term->line->data);
		return (TERM_ENL);
	}
	return (TERM_EEOF);
}

/*
**	Read and parse an interactive terminal's input.
*/
t_term_err	term_read_caps(t_term *term)
{
	t_term_err	status;
	ssize_t		read_st;
	char		c;

	status = TERM_EOK;
	term->line = term->hist.next;
	term->pos = 0;
	tputs(term->caps.mode.insert, 0, ft_putchar);
	while (status == TERM_EOK)
	{
		if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
			status = (read_st == 0) ? TERM_EEOF : TERM_EREAD;
		else if (c == TERM_ESC)
			status = term_read_esc(term);
		else if (ft_iscntrl(c))
			status = term_cntrl(term, c);
		else if (!line_insert(term->line, term->pos, &c, 1))
			status = TERM_EALLOC;
		else if (write(STDERR_FILENO, &c, 1) == -1)
			status = TERM_EWRITE;
		else
			term->pos++;
	}
	tputs(term->caps.mode.insert_end, 0, ft_putchar);
	return (status);
}