#include "term.h"

int	putc_err(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}
