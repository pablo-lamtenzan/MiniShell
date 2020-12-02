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
	char	*clear;
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
	char	*move;
	char	*move_h;
	char	*up;
	char	*down;
	char	*left;
	char	*right;
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

typedef struct	s_caps
{
	struct termios	s_ios;
	struct termios	s_ios_orig;
	t_modes			mode;
	t_ctrls			ctrl;
	t_keys			key;
	t_flags			flag;
}				t_caps;

#endif
