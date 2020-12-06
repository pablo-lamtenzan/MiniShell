#include <term/term.h>

void		caps_goto_x(t_caps *caps, int pos)
{
	int			delta;
	char		*ctrl;

	if ((delta = caps->cursor.pos.x - pos))
	{
		if (caps->ctrls.move_h)
			tputs(tgoto(caps->ctrls.move_h, 0, pos), 1, &putc_err);
		else
		{
			if (delta < 0)
			{
				delta = -delta;
				ctrl = g_term.caps.ctrls.left;
			}
			else
				ctrl = g_term.caps.ctrls.right;
			while (delta--)
				tputs(ctrl, 1, &putc_err);
		}
	}
}

void		caps_goto_y(t_caps *caps, int pos)
{
	int			delta;
	char		*ctrl;

	if ((delta = caps->cursor.pos.y - pos))
	{
		if (caps->ctrls.move_v)
			tputs(tgoto(caps->ctrls.move_v, 0, pos), 1, &putc_err);
		else
		{
			if (delta < 0)
			{
				delta = -delta;
				ctrl = g_term.caps.ctrls.up;
			}
			else
				ctrl = g_term.caps.ctrls.down;
			ft_dprintf(2, "%d, ", delta);
			while (delta--)
				tputs(ctrl, 1, &putc_err);
		}
	}
}

/*
**	Set the cursor's real position to pos.
*/
void		caps_goto(t_caps *caps, t_pos *pos)
{
	const bool		insert = caps->mode & CAPS_MINS && !caps->flags.move_insert;

	if (insert)
		tputs(caps->modes.insert_end, 1, &putc_err);
	// if height is the same move only horizontally // TODO: remove false
	caps_goto_y(caps, pos->y);
	caps_goto_x(caps, pos->x);
	caps->cursor.real = *pos;
	if (insert)
		tputs(caps->modes.insert, 1, &putc_err);
}


/*
**	Set the cursor's position to origin + pos.
*/
/* void	cursor_goto(t_caps *caps, t_pos *pos)
{
	const bool		insert = caps->mode & CAPS_MINS && !caps->flags.move_insert;
	const size_t	delta_y = pos->y - caps->cursor.real.y;

	if (insert)
		tputs(caps->modes.insert_end, 1, &putc_err);
	// if height is the same move only horizontally
	if (!delta_y && caps->ctrls.move_h)
		tputs(tgoto(caps->ctrls.move_h, 0, pos->x), 1, &putc_err);
	else // else move from the delta
		tputs(tgoto(caps->ctrls.move, delta_y, pos->x), 1, &putc_err);
	caps->cursor.real = *pos;
	if (insert)
		tputs(caps->modes.insert, 1, &putc_err);
} */

/*
**	Delete n characters starting at the cursor's position.
*/
void		caps_delete(t_caps *caps, size_t n)
{
	bool	toggle_del;

	if (caps->ctrls.del_n)
	{
		tputs(tparm(caps->ctrls.del_n, 0, n), 1, &putc_err);
	}
	else if (caps->ctrls.del)
	{
		if ((toggle_del = !(caps->mode & CAPS_MDEL)))
			tputs(caps->modes.del, 1, &putc_err);
		while (n--)
			tputs(caps->ctrls.del, 1, &putc_err);
		if (toggle_del)
			tputs(caps->modes.del_end, 1, &putc_err);
	}
}
