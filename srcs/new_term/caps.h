#ifndef CAPS_H
# define CAPS_H

# include <sys/stat.h>

# include <term.h>
# define MAX_ENTRY		1024

typedef struct	s_modes
{
	const char	*insert;
	const char	*insert_end;
	const char	*del;
	const char	*del_end;
	const char	*clear;
	const char	*standout;
	const char	*standout_end;
}				t_modes;

typedef struct	s_ctrls
{
	const char	*del;
	const char	*del_n;
	const char	*del_line;
	const char	*erase_n;
	const char	*move;
	const char	*move_h;
	const char	*up;
	const char	*down;
	const char	*left;
	const char	*right;
}				t_ctrls;

typedef struct	s_keys
{
	char	*up;
	char	*down;
	char	*left;
	char	*right;
	char	*del;
}				t_keys;

typedef struct	s_caps
{
	struct termios	s_ios;
	struct termios	s_ios_orig;
	t_modes	mode;
	t_ctrls	ctrl;
	t_keys	key;
}				t_caps;

#endif
