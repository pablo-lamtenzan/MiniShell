#include <term/term.h>

t_term_err  select_next_word(void)
{
    if (g_term.pos < g_term.line->len)
    {
        if (g_term.selec.start == -1U || g_term.selec.end == -1U)
        {
            g_term.selec.start = g_term.pos;
            g_term.selec.end = g_term.pos;
        }
        cursor_next_word();
        g_term.selec.end = g_term.pos;
        select_highlight();
    }
    return (TERM_EOK);
}

t_term_err  select_prev_word(void)
{
    if (g_term.pos > 0)
    {
        if (g_term.selec.start == -1U || g_term.selec.end == -1U)
        {
            g_term.selec.start = g_term.pos;
            g_term.selec.end = g_term.pos;
        }
        cursor_prev_word();
        g_term.selec.start = g_term.pos;
        select_highlight();
    }
    return (TERM_EOK);
}
