#include "term.h"

# define TERM_RPT_DIG	6
# define TERM_RPT_PRE	"(arg: "
# define TERM_RPT_SUF	") "
# define TERM_RPT_MSG	TERM_RPT_PRE""TERM_RPT_SUF

static t_term_err	repeat_atoi(t_term *term, char *c, uint32_t *rep_dest)
{
	t_term_err	status;
	ssize_t		read_st;
	char		digits;
	size_t		repeat;

	status = term_write_msg(term, TERM_RPT_MSG, sizeof(TERM_RPT_MSG) - 1);
	if (status != TERM_EOK)
		return (status);
	digits = 0;
	repeat = 0;
	while (ft_isdigit(*c) && digits < TERM_RPT_DIG)
	{
		if (*c != '0' || digits == 0)
		{
			caps_goto(&term->caps, sizeof(TERM_RPT_PRE) - 1 + digits);
			if (write(STDERR_FILENO, c, sizeof(*c)) == -1)
				return (TERM_EWRITE);
			digits++;
			term->origin++;
			caps_goto(&term->caps, term->origin + term->pos);
			repeat = (10 * repeat) + (*c - '0');
		}
		if ((read_st = read(0, c, sizeof(*c))) != sizeof(*c))
			return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	}
	status = term_write_msg(term, term->msg, ft_strlen(term->msg));
	if (digits == TERM_RPT_DIG)
		*rep_dest = 0;
	else
		*rep_dest = repeat;
	return (status);
}

/*
**	Read an escaped repetition count and repeat the following character.
*/
static t_term_err	term_read_repeat(t_term *term, char c)
{
	t_term_err	status;
	uint32_t	repeat;
	char		*repetition;

	status = TERM_EOK;
	if ((status = repeat_atoi(term, &c, &repeat)) == TERM_EOK)
	{
		if (c == TERM_NL)
			status = term_new_line(term);
		else if (repeat == 0)
			status = TERM_EOK;
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
	if (ft_isdigit(c))
		return (term_read_repeat(term, c));
	if (c == 'b')
		return (cursor_prev_word(term));
	if (c == 'f')
		return (cursor_next_word(term));
	ft_dprintf(2, "Unhandled ESC: %c(%2x)!\n",c, c);
	//ft_dprintf(2, "[PROMPT][ESC][RPT] %d\n", repeat);
	return (TERM_EOK);
}
