/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 19:28:58 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 01:51:37 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_err	clip_copy(void)
{
	if (g_term.caps.selec.start != -1U && g_term.caps.selec.end != -1U)
	{
		free(g_term.caps.clip.data);
		g_term.caps.clip.data = NULL;
		g_term.caps.clip.len = g_term.caps.selec.end - g_term.caps.selec.start;
		if (!g_term.caps.clip.len || !g_term.line)
			return (TERM_EOK);
		if (!(g_term.caps.clip.data = ft_substr(g_term.line->data,
		g_term.caps.selec.start, g_term.caps.clip.len)))
			return (TERM_EALLOC);
		cursor_goto_index(g_term.caps.selec.start);
		g_term.caps.clip.size = g_term.caps.clip.len + 1;
		g_term.caps.clip.next = NULL;
		g_term.caps.clip.prev = NULL;
	}
	return (TERM_EOK);
}

t_term_err	clip_cut(void)
{
	t_term_err	status;

	if (g_term.caps.selec.start != -1U && g_term.caps.selec.end != -1U
	&& (status = clip_copy()) == TERM_EOK)
	{
		//ft_dprintf(2, "start: %lu len: %lu\n", g_term.caps.selec.start, g_term.clip.len);
		//caps_goto(&g_term.caps, g_term.caps.origin + g_term.pos);
		// TODO: Fix line_erase with full line (Actually it seems to work...)
		term_line_del(g_term.caps.clip.len);
		select_clear();
	}
	return (TERM_EOK);
}

// TODO: Use term_write!
t_term_err	clip_paste(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.clip.len)
	{
		select_clear();
		if (!line_insert(g_term.line, g_term.caps.index, g_term.caps.clip.data, g_term.caps.clip.len))
			status = TERM_EALLOC;
		else
			status = term_write(g_term.caps.clip.data, g_term.caps.clip.len);
	}
	return (status);
}

t_term_err	clip_clear(void)
{
	g_term.caps.clip.len = 0;
	g_term.caps.clip.size = 0;
	free(g_term.caps.clip.data);
	g_term.caps.clip.data = NULL;
	return (TERM_EOK);
}
