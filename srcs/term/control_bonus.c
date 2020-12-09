#include <term/term.h>
#include <job_control.h>

/*
**	Delete one character to the left of the cursor.
*/
t_term_err	term_backspace()
{
	if (g_term.caps.index > 0)
	{
		cursor_l();
		term_line_del(1);
	}
	return (TERM_EOK);
}

/*
**	Cancel the terminal's input line.
*/
t_term_err	term_interrupt(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) -1) == -1)
		return (TERM_EWRITE);
	if (!g_term.msg)
		g_term.caps.cursor.origin = g_term.caps.cursor.zero;
	else if ((status = term_origin(g_term.msg->data, g_term.msg->len)) != TERM_EOK)
		return (status);
	term_line_discard();
	return (status);
}

/*
**	End transmition when the line is empty and there are no jobs to be awaited.
*/
t_term_err	term_eof(void)
{
	if (g_term.line->len == 0)
	{
		if (g_session.exit_count == 1 || !is_background_stopped())
			return (TERM_EEOF);
		else
		{
			if (g_session.exit_count == 0)
			{
				// TO DO: sleep 2 ; bg ; (wait 2 secs) ctrl^D -> inf loop
				// TO DO: print prompt segfaults (heap overflow)
				write(STDERR_FILENO, TERM_STOPPED_JOBS, sizeof(TERM_STOPPED_JOBS) - 1);
				if (g_term.msg)
					write(STDERR_FILENO, g_term.msg->data, g_term.msg->len);
			}
			g_session.exit_count++;
		}
	}
	return (TERM_EOK);
}
