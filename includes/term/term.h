#ifndef TERM_H
# define TERM_H

// TODO: Migrate headers
// TODO: ONOEOT (Discard ^D)
# include <unistd.h>

# include <env.h>
# include <term/line.h>
# include <term/caps.h>

# define TERM_DEV_NULL	"/dev/null"

/*
**	The starting size of a newly allocated empty line.
*/
# define TERM_LINE_SIZE	8

# define C_D			"\033[39m"
# define C_GL			"\033[92m"
# define C_B			"\033[34m"
# define C_BL			"\033[94m"
# define C_Y			"\033[33m"
# define S_D			"\033[0m"
# define S_B			"\033[1m"

/*
**	Displayed before reading the first-line of a command.
*/
# define TERM_PS1		C_D"["S_B""C_B"$USER"S_D""C_D"]"C_GL"$"C_D" "

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
# define TERM_EXIT		"exit"TERM_ENDL

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
	char		*msg;
	size_t		msg_len;
	size_t		origin;
	size_t		pos;
	t_clip		clip;
}					t_term;

t_term				g_term;

typedef t_term_err	(*t_term_action)(void);

typedef struct		s_keybind
{
	char			key;
	t_term_action	action;
}					t_keybind;

/*
**					term.c
*/
bool				term_init(t_env **env);
void				term_destroy(void);
t_term_err			term_prompt(const char **dest);

/*
**					write.c
*/
int					putc_err(int c);
size_t				strglen(const char *str);
t_term_err			term_write(const char *input, size_t length);

/*
**					caps.c
*/
bool				term_init_caps(t_env **env);


/*
**					caps_utils.c
*/
void				caps_goto(t_caps *caps, size_t pos);
void				caps_delete(t_caps *caps, size_t n);

/*
**					keybind.c
*/
t_term_action		keybind_get(const t_keybind *keybinds, size_t n, char key);

/*
**					read.c
*/
t_term_err			term_read_caps(void);
t_term_err			term_read(void);

/*
**					read_esc.c
*/
t_term_err			term_read_esc(void);

/*
**					read_csi.c
*/
t_term_err			term_read_csi(void);

/*
**					controls.c
*/
t_term_err			term_clear_screen(void);
t_term_err			term_backspace(void);
t_term_err			term_clear_line(void);
t_term_err			term_new_line(void);

/*
**					cursor.c
*/
t_term_err			cursor_l(void);
t_term_err			cursor_r(void);

/*
**					cursor_jmp.c
*/
t_term_err			cursor_start_line(void);
t_term_err			cursor_end_line(void);
t_term_err			cursor_next_word(void);
t_term_err			cursor_prev_word(void);

/*
**					select.c
*/
t_term_err			select_highlight(void);
t_term_err			select_left(void);
t_term_err			select_right(void);
t_term_err			select_clear(void);

/*
**					select_jmp.c
*/
t_term_err			select_next_word(void);
t_term_err			select_prev_word(void);

/*
**					clip.c
*/
t_term_err			clip_copy(void);
t_term_err			clip_cut(void);
t_term_err			clip_paste(void);
t_term_err			clip_clear(void);

/*
**					hist_cursor.c
*/
t_term_err			term_prev_line(void);
t_term_err			term_next_line(void);


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
t_term_err			term_interrupt(void);
t_term_err			term_eof(void);
t_term_err			term_stop(void);
t_term_err			term_suspend(void);

#endif
