#include <term/term.h>

void		caps_goto_x(t_caps *caps, int pos)
{
	int			delta;
	char		*ctrl;

	if ((delta = pos - caps->cursor.real.x))
	{
		caps->cursor.real.x = pos;
		if (caps->ctrls.move_h)
			tputs(tgoto(caps->ctrls.move_h, 0, pos), 1, &putc_err);
		else
		{
			if (delta < 0)
			{
				//ft_dprintf(2, "left * %d", delta);
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

	if ((delta = pos - caps->cursor.real.y))
	{
		caps->cursor.real.y = pos;
		if (delta < 0)
		{
			delta = -delta;
			ctrl = g_term.caps.ctrls.up;
		}
		else
			ctrl = g_term.caps.ctrls.down;
		//ft_dprintf(2, "%d, ", delta);
		while (delta--)
			tputs(ctrl, 1, &putc_err);
	}
}

// TODO: Do not use reference if sizeof(t_pos) is <= sizeof(t_pos*)
/*
**	Set the cursor's real position to pos.
*/
void		caps_goto(t_caps *caps, const t_pos *pos)
{
	const bool		insert = caps->mode & CAPS_MINS && !caps->flags.move_insert;

	if (insert)
		tputs(caps->modes.insert_end, 1, &putc_err);
	// if height is the same move only horizontally // TODO: remove false
	caps_goto_y(caps, pos->y);
	caps_goto_x(caps, pos->x);
	if (insert)
		tputs(caps->modes.insert, 1, &putc_err);
}
