#include <term.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ESC			'\033'
#define CSI			'['
#define IS_PARAM(x)	(((x) & 0xF0) == 0x30)
#define IS_INTER(x) (((x) & 0xF0) == 0x20)
#define IS_FINAL(x) (((x) & 0xC1) == 0x40)

#define TC_DISABLE	(ECHO | ICANON)	

typedef struct	s_csi
{
	char	param;
}				t_csi;

ssize_t	parse_esc(void)
{
	ssize_t	read_st;
	char	c;

	dprintf(STDERR_FILENO, "[ESC]:");
	if ((read_st = read(STDIN_FILENO, &c, sizeof(c)) == sizeof(c)))
	{
		if (c == CSI)
		{
			dprintf(STDERR_FILENO, "[CSI]:");
			while ((read_st = read(STDIN_FILENO, &c, sizeof(c))) == sizeof(c)
			&& IS_PARAM(c))	
			{
				dprintf(STDERR_FILENO, "[P]%c", c);
			}
			dprintf(2, "[UNK]%c", c);
			while (IS_INTER(c))	
			{
				dprintf(STDERR_FILENO, "[I]%c", c);
				if ((read_st = read(STDIN_FILENO, &c, sizeof(c))) != sizeof(c))
					return (read_st);
			}
			dprintf(2, "[UNK]%c", c);
			if (IS_FINAL(c))
			{
				dprintf(STDERR_FILENO, "[F]%c", c);
			}

		}
		else
		{
			dprintf(2, "[UNK]%c", c);
		}
	}
	return (read_st);
}

int		main(void)
{
	struct termios	options;
	ssize_t			read_st;
	char			c;
	int				orig_lflag;

	if (tcgetattr(STDIN_FILENO, &options) < 0)
	{
		perror("tcgetattr");
		return (1);
	}

	orig_lflag = options.c_lflag;
	options.c_lflag &= ~TC_DISABLE;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &options) < 0)
	{
		perror("tcsetattr");
		return (1);
	}

	while((read_st = read(STDIN_FILENO, &c, sizeof(c)) == sizeof(c)
	&& (c != ESC || (read_st = parse_esc()) == sizeof(c))));

	if (read_st != 0)
		perror("read");

	options.c_lflag = orig_lflag;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &options) < 0)
	{
		perror("tcsetattr");
		return (1);
	}
	return (read_st);
}
