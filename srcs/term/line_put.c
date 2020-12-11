#include <term/line.h>

bool	line_puts(t_line *line, const char *str)
{
	const size_t	len = ft_strlen(str);

	return (len ? line_insert(line, line->len, str, len) : true);
}

bool	line_putc(t_line *line, char c)
{
	return (line_insert(line, line->len, &c, 1));
}
