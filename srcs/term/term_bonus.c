/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/13 01:50:28 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

static void			term_resize_window(int signal)
{
	struct winsize	s_winsz;
	int				index;

	(void)signal;
	if (ioctl(0, TIOCGWINSZ, &s_winsz) != -1)
	{
		g_term.caps.width = s_winsz.ws_col;
		g_term.caps.height = s_winsz.ws_row;
		index = g_term.caps.index;
		caps_goto(&g_term.caps, (t_pos){0, 0});
		g_term.caps.index = 0;
		tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
		term_origin(g_term.msg->data, g_term.msg->len);
		term_write(g_term.line->data, g_term.line->len);
		cursor_goto_index(index);
	}
}

/*
**	Detect and init the terminal's capabilities
**
**	returns true if successful, or false otherwise.
*/

static t_term_err	term_init_caps(const char *term_type, bool is_login)
{
	char				term_buff[MAX_ENTRY + 1];
	int					ent_st;

	if (!term_type || (ent_st = tgetent(term_buff, term_type)) == 0)
	{
		return (write(STDERR_FILENO, "Could not detect terminal type!", 31)
			== -1 ? TERM_EWRITE : TERM_EOK);
	}
	if (ent_st == -1)
		return (TERM_ESETATTR);
	if (tcgetattr(STDIN_FILENO, &g_term.caps.s_ios) == -1)
		return (TERM_EGETATTR);
	g_term.caps.s_ios_orig = g_term.caps.s_ios;
	g_term.caps.s_ios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG);
	g_term.caps.s_ios.c_cflag |= ONLCR;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios) == -1)
		return (TERM_ESETATTR);
	g_term.has_caps = caps_load(&g_term.caps, is_login, &term_resize_window);
	return (true);
}

t_term_err			term_init(t_env **env, const char *cwd, bool is_login)
{
	t_term_err	status;

	if ((g_term.line = line_new(TERM_LINE_SIZE)))
	{
		status = TERM_EOK;
		g_term.caps.selec = (t_select){-1U, -1U};
		g_term.line->prev = g_term.caps.hist.last;
		g_term.caps.hist.curr = g_term.line;
		g_term.caps.hist.next = g_term.line;
		g_term.caps.hist.head = g_term.line;
		g_term.is_interactive = isatty(STDIN_FILENO) && isatty(STDERR_FILENO);
		if (g_term.is_interactive
		&& (status = term_init_env(env, cwd)) == TERM_EOK)
			status = term_init_caps(env_get(*env, "TERM", 4), is_login);
	}
	else
		status = TERM_EALLOC;
	return (status);
}

void				term_destroy(void)
{
	if (g_term.caps.hist.next != g_term.line)
		line_clear(&g_term.caps.hist.next);
	if (g_term.caps.hist.last != g_term.line)
		line_clear(&g_term.line);
	hist_clear(&g_term.caps.hist);
	clip_clear();
	line_clear(&g_term.msg);
	if (g_term.is_interactive)
		write(STDOUT_FILENO, TERM_EXIT, sizeof(TERM_EXIT) - 1);
}

/*
**	Prompt the user of an interactive terminal.
*/

t_term_err			term_prompt(const char **dst)
{
	t_term_err	status;

	status = TERM_EOK;
	if (!(g_term.is_interactive && g_term.msg->len)
	|| (status = term_origin(g_term.msg->data, g_term.msg->len)) == TERM_EOK)
	{
		status = (g_term.has_caps) ? term_read_caps() : term_read();
		if (dst)
			*dst = g_term.line->data;
	}
	return (status);
}
