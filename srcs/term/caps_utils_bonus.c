#include <term/caps.h>

/*
**	Put a character to the standard-error output.
*/
int			putc_err(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/*
**	Delete n characters starting at the cursor's position.
*/
void		caps_delete(t_caps *caps, size_t n)
{
	bool	toggle_del;

	if (n > 1 && caps->ctrls.del_n)
		tputs(tparm(caps->ctrls.del_n, 0, n), 1, &putc_err);
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
