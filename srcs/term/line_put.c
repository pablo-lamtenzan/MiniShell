#include <term/line.h>

bool	line_puts(t_line *line, const char *str)
{
	return (line_insert(line, line->len, str, ft_strlen(str)));
}

bool	line_putc(t_line *line, char c)
{
	return (line_insert(line, line->len, &c, 1));
}
