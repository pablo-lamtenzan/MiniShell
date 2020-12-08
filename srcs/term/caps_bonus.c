/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:53:27 by chamada           #+#    #+#             */
/*   Updated: 2020/12/07 17:28:29 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <sys/ioctl.h>
#include <signals.h>

static bool	load_modes(t_modes *modes, char **area)
{
	*modes = (t_modes) {
		tgetstr("im", area), tgetstr("ei", area), tgetstr("dm", area),
		tgetstr("ed", area), tgetstr("so", area), tgetstr("se", area),
	};
	return (modes->insert && modes->insert_end);
}

static bool	load_ctrls(t_ctrls *ctrls, char **area)
{
	*ctrls = (t_ctrls) {
		tgetstr("dc", area), tgetstr("DC", area), tgetstr("ce", area),
		tgetstr("cd", area), tgetstr("ec", area), tgetstr("cl", area),
		tgetstr("cm", area), tgetstr("ch", area), tgetstr("cv", area),
		tgetstr("up", area), tgetstr("do", area), tgetstr("le", area),
		tgetstr("nd", area), tgetstr("sf", area),
	};
	return (ctrls->del && ctrls->del_eol && ctrls->del_eos && ctrls->move
		&& ctrls->up && ctrls->down && ctrls->left && ctrls->right);
}

static bool	load_keys(t_keys *keys, char **area)
{
	*keys = (t_keys) {
		tgetstr("ku", area), tgetstr("kd", area), tgetstr("kl", area),
		tgetstr("kr", area), tgetstr("kD", area),
	};
	return (keys->up && keys->down && keys->left && keys->right);
}

static bool	load_flags(t_flags *flags)
{
	*flags = (t_flags) {
		tgetflag("mi"), tgetflag("bw"), tgetflag("am"),
	};
	return (true);
}

// TODO: Maybe handle errors (terminate)
static void		update_dimensions(int signal)
{
	struct winsize	s_winsz;
	int				index;

	(void)signal;
	if (ioctl(0, TIOCGWINSZ, &s_winsz) != -1)
	{
		g_term.caps.width = s_winsz.ws_col;
		g_term.caps.height = s_winsz.ws_row;
		index = g_term.caps.index;
		caps_goto(&g_term.caps, &g_term.caps.cursor.zero);
		g_term.caps.index = 0;
		tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
		term_origin(g_term.msg->data, g_term.msg->len);
		term_write(g_term.line->data, g_term.line->len);
		cursor_goto_index(index);
	}
}

/*
**	Detect and load the terminal's capabilities.
**
**	returns true if the terminal appears to be supported or false otherwise.
*/
bool		caps_load(t_caps *caps)
{
	bool	enabled;
	char	*area;

	area = NULL;
	enabled = load_modes(&caps->modes, &area)
		&& load_ctrls(&caps->ctrls, &area)
		&& load_keys(&caps->keys, &area)
		&& load_flags(&caps->flags)
		&& (caps->width = tgetnum("co")) > 0
		&& (caps->height = tgetnum("li")) > 0;
	if (enabled)
		signal(SIGWINCH, &update_dimensions);
	return (enabled);
}

