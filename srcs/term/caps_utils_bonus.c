/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/12 16:33:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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

	if (caps->ctrls.del_n)
		tputs(tparm(caps->ctrls.del_n, n), 1, &putc_err);
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

ssize_t		caps_insert(t_caps *caps, const char *input, size_t length)
{
	ssize_t		ret;
	const bool	toggle_ins = !(caps->mode & CAPS_MINS);

	ret = 0;
	if (toggle_ins)
		tputs(caps->modes.insert, 1, &putc_err);
	while (ret != -1 && length--)
	{
		tputs(caps->ctrls.ins, 1, &putc_err);
		ret = write(STDERR_FILENO, input++, 1);
		tputs(caps->ctrls.ins_pad, 1, &putc_err);
	}
	if (toggle_ins)
		tputs(caps->modes.insert_end, 1, &putc_err);
	return (ret);
}
