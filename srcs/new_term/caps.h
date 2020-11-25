#ifndef CAPS_H
# define CAPS_H

# include <sys/stat.h>

# include <term.h>
# define MAX_ENTRY		1024

typedef struct	s_modes
{
	char	*insert;
	char	*insert_end;
	char	*clear;
	char	*standout;
	char	*standout_end;
}				t_modes;

typedef struct	s_ctrls
{
	char	*del;
	char	*del_n;
	char	*del_line;
	char	*move;
	char	*move_h;
	char	*up;
	char	*down;
	char	*left;
	char	*right;
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
