#ifndef TERM_H
# define TERM_H

// TODO: Migrate headers
// TODO: ONOEOT (Discard ^D)
# include <unistd.h>

# include <env.h>
# include <term/caps.h>

# ifndef STDIN_FILENO
#  define STDIN_FILENO	0
# endif
# ifndef STDOUT_FILENO
#  define STDOUT_FILENO	1
# endif
# ifndef STDERR_FILENO
#  define STDERR_FILENO	2
# endif

# define TERM_DEV_NULL	"/dev/null"

/*
**	The starting size of a newly allocated empty line.
*/
# define TERM_LINE_SIZE	8

/*
**	Displayed before reading the first-line of a command.
*/
# define TERM_PS1		C_D"["S_B""C_BRI"$USER"S_D""C_D""C_BYI"@"S_BGI"$DIRNAME"S_D"]"C_W"$"C_D" "

/*
**	Displayed before reading the second and subsquent lines of a command.
*/
# define TERM_PS2		"> "

# define TERM_NL		'\n'

# define TERM_ENDL		"\n"
# define TERM_EXIT		"exit"TERM_ENDL
# define TERM_STOPPED_JOBS TERM_ENDL"There are stopped jobs."TERM_ENDL

typedef enum		e_term_err
{
	TERM_ESETATTR	= -4,
	TERM_EALLOC		= -3,
	TERM_EWRITE		= -2,
	TERM_EREAD		= -1,
	TERM_EEOF		= 0,
	TERM_EOK		= 1,
	TERM_ENL		= 2
}					t_term_err;

typedef struct		s_term
{
	t_caps		caps;
	bool		is_interactive;
	bool		has_caps;
	t_line		*line;
	t_line		*msg;
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
bool				term_init(t_env **env, const char *cwd);
void				term_destroy(void);
t_term_err			term_prompt(const char **dest);

/*
**					write.c
*/
int					putc_err(int c);
size_t				strglen(const char *str);
t_term_err			cursor_write(const char *input, size_t length);
t_term_err			term_write(const char *input, size_t length);
t_term_err			term_origin(const char *input, size_t length);

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
**					clear_bonus.c
*/
t_term_err			term_clear_screen(void);
t_term_err			term_clear_eos(void);
t_term_err			term_clear_line(void);

/*
**					controls.c
*/
t_term_err			term_line_new(void);
t_term_err			term_line_del(size_t n);
t_term_err			term_backspace(void);
t_term_err			term_line_discard(void);
t_term_err			term_line_kill(void);

/*
**					cursor.c
*/
t_term_err			cursor_l(void);
t_term_err			cursor_r(void);
t_term_err			cursor_d(void);

/*
**					cursor_jmp.c
*/
void				cursor_goto_index(size_t index);
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
**					signals.c
*/
t_term_err			term_interrupt(void);
t_term_err			term_eof(void);
t_term_err			term_stop(void);
t_term_err			term_suspend(void);

#endif
