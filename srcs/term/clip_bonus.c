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
	if (g_term.selec.start != -1U && g_term.selec.end != -1U)
	{
		free(g_term.clip.data);
		g_term.clip.data = NULL;
		g_term.clip.len = g_term.selec.end - g_term.selec.start;
		if (!g_term.clip.len || !g_term.line)
			return (TERM_EOK);
		if (!(g_term.clip.data = ft_substr(g_term.line->data,
		g_term.selec.start, g_term.clip.len)))
			return (TERM_EALLOC);
		g_term.pos = g_term.selec.start;
		g_term.clip.size = g_term.clip.len + 1;
		g_term.clip.next = NULL;
		g_term.clip.prev = NULL;
	}
	return (TERM_EOK);
}

t_term_err	clip_cut(void)
{
	t_term_err	status;

	if (g_term.selec.start != -1U && g_term.selec.end != -1U)
	{
		if ((status = clip_copy()) != TERM_EOK)
			return (status);
		//ft_dprintf(2, "start: %lu len: %lu\n", g_term.selec.start, g_term.clip.len);
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
		caps_delete(&g_term.caps, g_term.clip.len);
		// TODO: Fix line_erase with full line (Actually it seems to work...)
		line_erase(g_term.line, g_term.pos, g_term.clip.len);
		select_clear();
	}
	return (TERM_EOK);
}

t_term_err	clip_paste(void)
{
	if (!g_term.clip.len)
		return (TERM_EOK);
	select_clear();
	if (write(STDERR_FILENO, g_term.clip.data, g_term.clip.len) == -1)
		return (TERM_EWRITE);
	if (!line_insert(g_term.line, g_term.pos, g_term.clip.data, g_term.clip.len))
		return (TERM_EALLOC);
	g_term.pos += g_term.clip.len;
	return (TERM_EOK);
}

t_term_err	clip_clear(void)
{
	g_term.clip.len = 0;
	g_term.clip.size = 0;
	free(g_term.clip.data);
	g_term.clip.data = NULL;
	return (TERM_EOK);
}
