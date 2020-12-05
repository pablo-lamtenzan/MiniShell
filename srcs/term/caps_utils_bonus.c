#include <term/term.h>

/*
**	Set the cursor's real position to pos.
*/
void		caps_goto(t_caps *caps, t_pos *pos)
{
	const bool		insert = caps->mode & CAPS_MINS && !caps->flags.move_insert;
	const size_t	delta_y = pos->y - caps->cursor.real.y;

	ft_dprintf(2, "%lu", delta_y);
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
