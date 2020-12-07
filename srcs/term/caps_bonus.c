/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:53:27 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 01:50:05 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

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

/*
**	Detect and load the terminal's capabilities.
**
**	returns true if the terminal appears to be supported or false otherwise.
*/
bool		caps_load(t_caps *caps)
{
	char	*area;

	area = NULL;
	return (load_modes(&caps->modes, &area)
		&& load_ctrls(&caps->ctrls, &area)
		&& load_keys(&caps->keys, &area)
		&& load_flags(&caps->flags)
		&& (caps->width = tgetnum("co")) > 0
		&& (caps->height = tgetnum("li")) > 0);
}
