/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_destructors2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 07:09:15 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:36:18 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void				delete_zombies(void)
{
	t_background	*fill;

	while (g_session.zombies)
	{
		fill = g_session.zombies;
		g_session.zombies = g_session.zombies->next;
		free(fill);
		fill = NULL;
	}
}

void				delete_hist(void)
{
	t_history		*fill;

	while (g_session.hist)
	{
		fill = g_session.hist;
		g_session.hist = g_session.hist->next;
		free(fill);
		fill = NULL;
	}
}

void				delete_input_line(void)
{
	int				i;

	i = -1;
	while (g_session.input_line && ++i < matrix_height(g_session.input_line))
		free(g_session.input_line[i]);
	free(g_session.input_line);
	g_session.input_line = NULL;
}
