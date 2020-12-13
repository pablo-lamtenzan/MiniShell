/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 18:57:18 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 00:17:25 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

void	hist_add(t_hist *hist, t_line *line)
{
	if (hist->last)
		hist->last->next = line;
	line->prev = hist->last;
	line->next = hist->next;
	*hist->next->data = '\0';
	hist->next->len = 0;
	hist->last = line;
	hist->next->prev = line;
	hist->curr = hist->next;
}

void	hist_pop(t_hist *hist)
{
	t_line	*prev;

	if (hist->last)
	{
		prev = hist->last->prev;
		line_clear(&hist->last);
		hist->last = prev;
	}
}

void	hist_clear(t_hist *hist)
{
	t_line	*curr;

	while ((curr = hist->last))
	{
		hist->last = curr->prev;
		line_clear(&curr);
	}
}

void	hist_commit(t_hist *hist, t_line *line)
{
	free(hist->curr->data);
	hist->curr->data = line->data;
	hist->curr->len = line->len;
	hist->curr->size = line->size;
}
