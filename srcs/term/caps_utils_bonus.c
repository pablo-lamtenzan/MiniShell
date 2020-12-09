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

ssize_t		caps_insert(t_caps *caps, const char* input, size_t length)
{
	ssize_t	ret;
	bool	toggle_ins;

	ret = 0;
	if (caps->ctrls.ins_n)
	{
		tputs(tparm(caps->ctrls.ins_n, length), 1, &putc_err);
		ret = write(STDERR_FILENO, input, length);
	}
	else
	{
		if ((toggle_ins = !(caps->mode & CAPS_MINS)))
			tputs(caps->modes.insert, 1, &putc_err);
		while (ret != -1 && length--)
		{
			tputs(caps->ctrls.ins, 1, &putc_err);
			ret = write(STDERR_FILENO, input++, 1);
			tputs(caps->ctrls.ins_pad, 1, &putc_err);
		}
		if (toggle_ins)
			tputs(caps->modes.insert_end, 1, &putc_err);
	}
	return (ret);
}
