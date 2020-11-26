#include "term.h"

t_term_err  select_next_word(t_term *term)
{
    if (term->pos < term->line->len)
    {
        if (term->clip.select.start == -1U || term->clip.select.end == -1U)
        {
            term->clip.select.start = term->pos;
            term->clip.select.end = term->pos;
        }
        cursor_next_word(term);
        term->clip.select.end = term->pos;
        select_highlight(term);
    }
    return (TERM_EOK);
}

t_term_err  select_prev_word(t_term *term)
{
    if (term->pos < term->line->len)
    {
        if (term->clip.select.start == -1U || term->clip.select.end == -1U)
        {
            term->clip.select.start = term->pos;
            term->clip.select.end = term->pos;
        }
        cursor_prev_word(term);
        term->clip.select.start = term->pos;
        select_highlight(term);
    }
    return (TERM_EOK);
}
