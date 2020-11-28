#include <term/term.h>
/*
**	Set the cursor to pos on the horizontal axis.
*/
void		caps_goto(t_caps *caps, size_t pos)
{
	tputs(caps->mode.insert_end, 1, &putc_err);
	if (caps->ctrl.move_h)
		tputs(tgoto(caps->ctrl.move_h, 0, pos), 1, &putc_err);
	else
		tputs(tgoto(caps->ctrl.move, 0, pos), 1, &putc_err);
	tputs(caps->mode.insert, 1, &putc_err);
}

/*
**	Delete n characters starting at the cursor's position
*/
void		caps_delete(t_caps *caps, size_t n)
{
	if (caps->ctrl.del)
	{
		tputs(caps->mode.del, 1, &putc_err);
		while (n--)
			tputs(caps->ctrl.del, 1, &putc_err);
		tputs(caps->mode.del_end, 1, &putc_err);
	}
}
