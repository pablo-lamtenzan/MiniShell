#ifndef CAPS_H
# define CAPS_H

# include <sys/stat.h>

# include <term.h>
# define MAX_ENTRY		1024


/*
**	Special output-sequences to toggle input-modes.
*/
typedef struct	s_modes
{
	char	*insert;
	char	*insert_end;
	char	*del;
	char	*del_end;
	char	*standout;
	char	*standout_end;
}				t_modes;

/*
**	Special output-sequences to control the terminal.
*/
typedef struct	s_ctrls
{
	char	*del;
	char	*del_n;
	char	*del_line;
	char	*erase_n;
	char	*clear;
	char	*move;
	char	*move_h;
	char	*move_v;
	char	*up;
	char	*down;
	char	*left;
	char	*right;
	char	*scroll_down;
}				t_ctrls;


// TODO: Check length of keys sequences (Could SEGFAULT)

/*
**	Special input-sequences emmited by the terminal.
*/
typedef struct	s_keys
{
	char	*up;
	char	*down;
	char	*left;
	char	*right;
	char	*del;
}				t_keys;

/*
**	move_insert: The cursor can be moved in insert mode.
**
**	wrap_back: The cursor can wrap back one line at column zero, when using le.
**
**	wrap_forward: The cursor can wrap forward one line when writing at the last
**	column.
*/
typedef struct	s_flags
{
	bool	move_insert;
	bool	wrap_back;
	bool	wrap_forward;
}				t_flags;

typedef uint8_t	t_mode;
# define CAPS_MNONE	0
# define CAPS_MINS	1
# define CAPS_MDEL	2
# define CAPS_MSO	4

typedef struct		s_pos
{
	int	x;
	int	y;
}					t_pos;

typedef struct		s_cursor
{
	t_pos	zero;
	t_pos	real;
	t_pos	origin;
	t_pos	pos;
}					t_cursor;

typedef struct		s_select
{
	size_t	start;
	size_t	end;
}					t_select;

typedef struct	s_caps
{
	struct termios	s_ios;
	struct termios	s_ios_orig;

	t_mode			mode;

	t_modes			modes;
	t_ctrls			ctrls;
	t_keys			keys;
	t_flags			flags;

	int				width;
	int				height;
	t_hist			hist;
	t_cursor		cursor;
	size_t			index;
	t_select		selec;
	t_line			clip;
}				t_caps;

/*
**					caps.c
*/
bool				caps_load(t_caps *caps);

/*
**					caps_utils.c
*/
void				caps_goto(t_caps *caps, t_pos *pos);
void				caps_delete(t_caps *caps, size_t n);

#endif
