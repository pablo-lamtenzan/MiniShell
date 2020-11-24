#include "term.h"

# define TERM_RPT_DIG	8
# define TERM_RPT_PRE	"(arg: "
# define TERM_RPT_SUF	") "
# define TERM_RPT_MSG	TERM_RPT_PRE""TERM_RPT_SUF

static t_term_err	repeat_atoi(t_term *term, char c, uint32_t *dest)
{
	t_term_err	status;
	ssize_t		read_st;
	char		digits;
	size_t		repeat;
	size_t		pos;

	status = term_write_msg(term, TERM_RPT_MSG, sizeof(TERM_RPT_MSG) - 1);
	if (status != TERM_EOK)
		return (status);
	pos = sizeof(TERM_RPT_PRE) - 1;
	digits = 1;
	repeat = 0;
	while (ft_isdigit(c) && ++digits < TERM_RPT_DIG)
	{
		tputs(tgoto(term->caps.ctrl.move_h, 0, pos), 0, &putc_err);
		if (write(STDERR_FILENO, &c, 1) == -1)
			return (TERM_EWRITE);
		tputs(tgoto(term->caps.ctrl.move_h, 0, term->pos), 0, &putc_err);
		pos++;
		term->origin++;
		repeat = (10 * repeat) + (c - '0');
		if ((read_st = read(0, &c, 1)) != 1)
			return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	}
	status = term_write_msg(term, term->msg, ft_strlen(term->msg));
	*dest = (repeat == 0) ? 1 : repeat;
	return (status);
}

static t_term_err	term_read_repeat(t_term *term, char c)
{
	t_term_err	status;
	ssize_t		read_st;
	uint32_t	repeat;
	char		*repetition;

	status = TERM_EOK;
	if (ft_isdigit(c) && (status = repeat_atoi(term, c, &repeat)) == TERM_EOK)
	{
		if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
			status = (read_st == 0) ? TERM_EEOF : TERM_EREAD;
		else if (repeat == 1)
			status = term_write(term, &c, 1);
		else if ((repetition = malloc(sizeof(*repetition) * repeat)))
		{
			ft_memset(repetition, c, repeat);
			status = term_write(term, repetition, repeat);
			free(repetition);
		}
		else
			status = TERM_EALLOC;
	}
	return (status);
}

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
	//ft_dprintf(2, "[PROMPT][ESC][RPT] %d\n", repeat);
	return (term_read_repeat(term, c));
}
