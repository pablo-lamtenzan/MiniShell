#include <lexer.h>

t_token		*parse_token(const char **txt)
{
	t_operator		*new;
	t_operator_t	type;
	int				pos;

	new = NULL;
	if (pos = ft_strpos(OPERATORS, *(*txt++)) > 0)
	{
		if (!(new = malloc(sizeof(t_token))))
			return (NULL);
		if (pos == REDIRECTION_GREATHER && **txt == '>')
			type = REDIRECTION_DGREATHER;
		else
			type = (1 << pos);
		if (new = malloc(sizeof(*new))){
			new->type = type;
			new->next = NULL;
		}
	}
	return (new);
}
