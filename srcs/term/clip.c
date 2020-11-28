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
	if (g_term.clip.select.start != -1U && g_term.clip.select.end != -1U)
	{
		free(g_term.clip.line.data);
		g_term.clip.line.data = NULL;
		g_term.clip.line.len = g_term.clip.select.end - g_term.clip.select.start;
		if (!g_term.clip.line.len || !g_term.line)
			return (TERM_EOK);
		if (!(g_term.clip.line.data = ft_substr(g_term.line->data,
		g_term.clip.select.start, g_term.clip.line.len)))
			return (TERM_EALLOC);
		g_term.pos = g_term.clip.select.start;
		g_term.clip.line.size = g_term.clip.line.len + 1;
		g_term.clip.line.next = NULL;
		g_term.clip.line.prev = NULL;
	}
	return (TERM_EOK);
}

t_term_err	clip_cut(void)
{
	t_term_err	status;

	if (g_term.clip.select.start != -1U && g_term.clip.select.end != -1U)
	{
		if ((status = clip_copy()) != TERM_EOK)
			return (status);
		//ft_dprintf(2, "start: %lu len: %lu\n", g_term.clip.select.start, g_term.clip.line.len);
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
		caps_delete(&g_term.caps, g_term.clip.line.len);
		// TODO: Fix line_erase with full line (Actually it seems to work...)
		line_erase(g_term.line, g_term.pos, g_term.clip.line.len);
		select_clear();
	}
	return (TERM_EOK);
}

t_term_err	clip_paste(void)
{
	if (!g_term.clip.line.len)
		return (TERM_EOK);
	select_clear();
	if (write(STDERR_FILENO, g_term.clip.line.data, g_term.clip.line.len) == -1)
		return (TERM_EWRITE);
	if (!line_insert(g_term.line, g_term.pos, g_term.clip.line.data, g_term.clip.line.len))
		return (TERM_EALLOC);
	g_term.pos += g_term.clip.line.len;
	return (TERM_EOK);
}

t_term_err	clip_clear(void)
{
	g_term.clip.line.len = 0;
	g_term.clip.line.size = 0;
	free(g_term.clip.line.data);
	g_term.clip.line.data = NULL;
	return (TERM_EOK);
}
