/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 13:20:36 by chamada           #+#    #+#             */
/*   Updated: 2020/12/13 01:50:28 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM_H
# define TERM_H

# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

# include <env.h>
# include <term/caps.h>

/*
**	Default file-descriptors.
*/
# ifndef STDIN_FILENO
#  define STDIN_FILENO	0
# endif
# ifndef STDOUT_FILENO
#  define STDOUT_FILENO	1
# endif
# ifndef STDERR_FILENO
#  define STDERR_FILENO	2
# endif

/*
**	Character device file to exclude during tty-detection.
*/
# define TERM_DEV_NULL	"/dev/null"

/*
**	The starting size of a newly allocated empty line.
*/
# define TERM_LINE_SIZE	8

/*
**	Default new-line character.
*/
# define TERM_NL		'\n'

/*
**	Default line ending string.
*/
# define TERM_ENDL		"\n"

/*
**	Default exit message.
*/
# define TERM_EXIT		"exit\n"

/*
**	Exit message when there are stopped jobs.
*/
# define TERM_EXIT_JOBS	"\nThere are stopped jobs.\n"

/*
**	Terminal error codes.
*/
typedef enum		e_term_err
{
	TERM_ESETATTR = -7,
	TERM_EGETATTR = -6,
	TERM_EGETENT = -5,
	TERM_EALLOC = -3,
	TERM_EWRITE = -2,
	TERM_EREAD = -1,
	TERM_EEOF = 0,
	TERM_EOK = 1,
	TERM_ENL = 2
}					t_term_err;

typedef struct		s_term
{
	t_caps		caps;
	bool		is_interactive;
	bool		has_caps;
	t_line		*line;
	t_line		*msg;
}					t_term;

// TODO: Extern in main
t_term				g_term;

typedef t_term_err	(*t_term_action)(void);

typedef struct		s_keybind
{
	char			key;
	t_term_action	action;
}					t_keybind;

/*
**					term.c / term_bonus.c
*/
t_term_err			term_init(t_env **env, const char *cwd, bool is_login);
void				term_destroy(void);
t_term_err			term_prompt(const char **dest);

/*
**					init.c
*/
t_term_err			term_init_env(t_env **env, const char *cwd);
void				term_perror(t_term_err err);

/*
**					write_bonus.c
*/
int					putc_err(int c);
size_t				strglen(const char *str);
t_term_err			cursor_write(const char *input, size_t length);
t_term_err			term_write(const char *input, size_t length);
t_term_err			term_origin(const char *input, size_t length);

/*
**					keybind_bonus.c
*/
t_term_action		keybind_get(const t_keybind *keybinds, size_t n, char key);

/*
**					read.c / read_bonus.c
*/
t_term_err			term_read_caps(void);
t_term_err			term_read(void);

/*
**					read_esc_bonus.c
*/
t_term_err			term_read_esc(void);

/*
**					read_csi_bonus.c
*/
t_term_err			term_read_csi(void);
t_term_err			term_read_mod_none(void);
t_term_err			term_read_mod_shift(void);
t_term_err			term_read_mod_alt(void);
t_term_err			term_read_del(void);

/*
**					clear_bonus.c
*/
t_term_err			term_clear_screen(void);
t_term_err			term_clear_eos(void);
t_term_err			term_clear_line(void);

/*
**					control_bonus.c
*/
t_term_err			term_interrupt(void);
t_term_err			term_eof(void);

/*
**					control_line_bonus.c
*/
t_term_err			term_line_new(void);
t_term_err			term_line_del(size_t n);
t_term_err			term_backspace(void);
t_term_err			term_line_discard(void);
t_term_err			term_line_kill(void);

/*
**					cursor_bonus.c
*/
t_term_err			cursor_l(void);
t_term_err			cursor_r(void);
t_term_err			cursor_d(void);

/*
**					cursor_jmp_bonus.c
*/
void				cursor_goto_index(size_t index);
t_term_err			cursor_start_line(void);
t_term_err			cursor_end_line(void);
t_term_err			cursor_next_word(void);
t_term_err			cursor_prev_word(void);

/*
**					select_bonus.c
*/
t_term_err			select_highlight(void);
t_term_err			select_left(void);
t_term_err			select_right(void);
t_term_err			select_clear(void);

/*
**					select_jmp_bonus.c
*/
t_term_err			select_next_word(void);
t_term_err			select_prev_word(void);

/*
**					clip_bonus.c
*/
t_term_err			clip_copy(void);
t_term_err			clip_cut(void);
t_term_err			clip_paste(void);
t_term_err			clip_clear(void);

/*
**					hist_cursor_bonus.c
*/
t_term_err			term_prev_line(void);
t_term_err			term_next_line(void);

#endif
