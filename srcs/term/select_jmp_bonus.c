/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_jmp_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/06 00:03:30 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_err  select_next_word(void)
{
    if (g_term.caps.index < g_term.line->len)
    {
        if (g_term.caps.selec.start == -1U || g_term.caps.selec.end == -1U)
        {
            g_term.caps.selec.start = g_term.caps.index;
            g_term.caps.selec.end = g_term.caps.index;
        }
        cursor_next_word();
        g_term.caps.selec.end = g_term.caps.index;
        select_highlight();
    }
    return (TERM_EOK);
}

t_term_err  select_prev_word(void)
{
    if (g_term.caps.index > 0)
    {
        if (g_term.caps.selec.start == -1U || g_term.caps.selec.end == -1U)
        {
            g_term.caps.selec.start = g_term.caps.index;
            g_term.caps.selec.end = g_term.caps.index;
        }
        cursor_prev_word();
        g_term.caps.selec.start = g_term.caps.index;
        select_highlight();
    }
    return (TERM_EOK);
}
