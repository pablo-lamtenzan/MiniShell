#include "term.h"

t_term_err	term_prev_line(t_term *t)
{
	if (t->hist.curr->prev)
	{
		if (t->hist.curr != t->hist.next && t->line->len)
		{
			hist_commit(&t->hist, t->line);
			free(t->line);
		}
		t->hist.curr = t->hist.curr->prev;
		term_clear_line(t);
		if (write(1, t->hist.curr->data, t->hist.curr->len) == -1)
			return (TERM_EWRITE);
		t->line = line_dup(t->hist.curr);
		t->pos = t->hist.curr->len;
	}
	return (TERM_EOK);
}

t_term_err	term_next_line(t_term *t)
{
	if (t->hist.curr->next)
	{
		if (t->hist.curr != t->hist.next)
		{
			hist_commit(&t->hist, t->line);
			free(t->line);
		}
		t->hist.curr = t->hist.curr->next;
		term_clear_line(t);
		if (write(1, t->hist.curr->data, t->hist.curr->len) == -1)
			return (TERM_EWRITE);
		t->line = line_dup(t->hist.curr);
		t->pos = t->hist.curr->len;
	}
	return (TERM_EOK);
}
