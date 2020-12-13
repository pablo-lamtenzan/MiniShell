/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps_goto_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:34:40 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/09 16:34:40 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/caps.h>

void		caps_goto_x(t_caps *caps, int pos)
{
	int			delta;
	char		*ctrl;

	if ((delta = pos - caps->cursor.pos.x))
	{
		caps->cursor.pos.x = pos;
		if (caps->ctrls.move_h)
			tputs(tgoto(caps->ctrls.move_h, 0, pos), 1, &putc_err);
		else
		{
			if (delta < 0)
			{
				delta = -delta;
				ctrl = caps->ctrls.left;
			}
			else
				ctrl = caps->ctrls.right;
			while (delta--)
				tputs(ctrl, 1, &putc_err);
		}
	}
}

void		caps_goto_y(t_caps *caps, int pos)
{
	int			delta;
	char		*ctrl;

	if ((delta = pos - caps->cursor.pos.y))
	{
		if (delta < 0)
		{
			delta = -delta;
			ctrl = caps->ctrls.up;
		}
		else
			ctrl = caps->ctrls.down;
		while (delta--)
			tputs(ctrl, 1, &putc_err);
		caps->cursor.pos.y = pos;
	}
}
/*
**	Set the cursor's real position to pos.
*/

void		caps_goto(t_caps *caps, const t_pos pos)
{
	const bool		insert = caps->mode & CAPS_MINS && !caps->flags.move_insert;

	if (insert)
		tputs(caps->modes.insert_end, 1, &putc_err);
	caps_goto_y(caps, pos.y);
	caps_goto_x(caps, pos.x);
	if (insert)
		tputs(caps->modes.insert, 1, &putc_err);
}
