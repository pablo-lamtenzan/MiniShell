#include <lexer/lexer.h>
#include <env.h>

bool	is_special(char c)
{
	return (ft_strchr(LEX_SPEC, c) != NULL);
}
