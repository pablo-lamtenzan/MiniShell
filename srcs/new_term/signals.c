#include "term.h"

t_term_err	term_interrupt(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] INTERRUPT\n");
	return (TERM_EOK);
}

t_term_err	term_eof(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] EOF\n");
	if (t->line->len == 0)
		return (TERM_EEOF);
	return (TERM_EOK);
}

t_term_err	term_stop(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] STOP\n");
	return (TERM_EOK);
}

t_term_err	term_suspend(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] SUSPEND\n");
	return (TERM_EOK);
}
