#ifndef TERM_H
# define TERM_H

// TODO: Migrate headers
// TODO: ONOEOT (Discard ^D)
# include <unistd.h>

# include <env.h>
# include "line.h"
# include "caps.h"

# define TERM_DEV_NULL	"/dev/null"

/*
**	The starting size of a newly allocated empty line.
*/
# define TERM_LINE_SIZE	8

/*
**	Displayed before reading the first-line of a command.
*/
# define TERM_PS1		"minish> "

/*
**	Displayed before reading the second and subsquent lines of a command.
*/
# define TERM_PS2		"> "


# define TERM_ESC		'\033'
# define TERM_CSI		'['
# define TERM_ALTESC	'1'
# define TERM_SHIFT		'2'

# define TERM_CNTRL		'`'

# define TERM_NL		'\n'

# define TERM_ENDL		"\n"

typedef enum		e_term_err
{
	TERM_EALLOC = -3,
	TERM_EWRITE = -2,
	TERM_EREAD = -1,
	TERM_EEOF = 0,
	TERM_EOK = 1,
	TERM_ENL = 2
}					t_term_err;

typedef struct		s_select
{
	size_t	start;
	size_t	end;
}					t_select;

typedef struct		s_clip
{
	t_line		line;
	t_select	select;
}					t_clip;

typedef struct		s_hist
{
	t_line	*last;
	t_line	*curr;
	t_line	*next;
}					t_hist;

typedef struct		s_term
{
	t_caps		caps;
	bool		is_interactive;
	bool		has_caps;
	t_hist		hist;
	t_line		*line;
	const char	*msg;
	size_t		origin;
	size_t		pos;
	t_clip		clip;
}					t_term;

typedef t_term_err	(*t_term_action)(t_term *term);

typedef struct		s_keybind
{
	char			key;
	t_term_action	action;
}					t_keybind;

/*
**					term.c
*/
bool				term_init(t_term *term, t_env **env);
void				term_destroy(t_term *term);
t_term_err			term_prompt(t_term *term);

/*
**					write.c
*/
int					putc_err(int c);

/*
**					caps.c
*/
bool				term_init_caps(t_term *term, t_env **env);

/*
**					read.c
*/
t_term_err			term_read_caps(t_term *term);
t_term_err			term_read(t_term *term);

/*
**					read_esc.c
*/
t_term_err			term_read_esc(t_term *term);

/*
**					read_csi.c
*/
t_term_err			term_read_csi(t_term *term);

/*
**					controls.c
*/
t_term_err			term_clear_screen(t_term *term);
t_term_err			term_backspace(t_term *term);
t_term_err			term_clear_line(t_term *term);
t_term_err			term_new_line(t_term *term);

/*
**					cursor.c
*/
t_term_err			cursor_l(t_term *term);
t_term_err			cursor_r(t_term *term);
t_term_err			cursor_start_line(t_term *term);
t_term_err			cursor_end_line(t_term *term);

/*
**					select.c
*/
t_term_err			select_left(t_term *t);
t_term_err			select_right(t_term *t);
t_term_err			select_clear(t_term *t);

/*
**					clip.c
*/
t_term_err			clip_copy(t_term *t);
t_term_err			clip_cut(t_term *t);
t_term_err			clip_paste(t_term *t);
t_term_err			clip_clear(t_term *t);

/*
**					hist_cursor.c
*/
t_term_err			term_prev_line(t_term *t);
t_term_err			term_next_line(t_term *t);


/*
**					hist.c
*/
void				hist_add(t_hist *hist, t_line *line);
void				hist_pop(t_hist *hist);
void				hist_clear(t_hist *hist);
void				hist_commit(t_hist *hist, t_line *line);

/*
**					signals.c
*/
t_term_err			term_interrupt(t_term *t);
t_term_err			term_eof(t_term *t);
t_term_err			term_stop(t_term *t);
t_term_err			term_suspend(t_term *t);

#endif
