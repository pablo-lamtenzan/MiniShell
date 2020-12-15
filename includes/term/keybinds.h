/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybinds.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 01:50:28 by chamada           #+#    #+#             */
/*   Updated: 2020/12/15 01:33:22 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBINDS_H
# define KEYBINDS_H

# include <term/term.h>

typedef struct			s_keybinds
{
	t_keybind	cntrls[14];
	t_keybind	escs[3];
	t_keybind	shifts[2];
	t_keybind	alts[2];
	t_keybind	csis[7];
	t_keybind	mods[3];
}						t_keybinds;

static const t_keybinds	g_keybinds = {
	.cntrls = {
		{CINTR, &term_interrupt},
		{CERASE, &term_backspace},
		{CEOF, &term_eof},
		{CWERASE, &term_line_kill},
		{CKILL, &term_line_kill},
		{CTRL('h'), &term_backspace},
		{CTRL('l'), &term_clear_screen},
		{CTRL('j'), &term_line_new},
		{CTRL('a'), &cursor_start_line},
		{CTRL('e'), &cursor_end_line},
		{CTRL('y'), &clip_paste},
		{CTRL('k'), &clip_cut},
		{CTRL('p'), &term_prev_line},
		{CTRL('n'), &term_next_line},
	},
	.escs = {
		{ANSI_CSI, &term_read_csi},
		{'b', &cursor_prev_word},
		{'f', &cursor_next_word},
	},
	.shifts = {
		{'D', &select_left},
		{'C', &select_right},
	},
	.alts = {
		{'D', select_prev_word},
		{'C', select_next_word},
	},
	.csis = {
		{'H', &cursor_start_line},
		{'F', &cursor_end_line},
		{'A', &term_prev_line},
		{'B', &term_next_line},
		{'D', &cursor_l},
		{'C', &cursor_r},
		{'3', &term_read_del},
	},
	.mods = {
		{'1', &term_read_mod_none},
		{'2', &term_read_mod_shift},
		{'4', &term_read_mod_alt},
	},
};

#endif
