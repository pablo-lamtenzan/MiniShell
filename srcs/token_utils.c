#include <lexer.h>

t_token			*token_clear(t_token **tokens, bool free_data)
{
	t_token	*next;

	while (*tokens)
	{
		next = (*tokens)->next;
		if (free_data)
			free((*tokens)->data);
		free(*tokens);
		*tokens = next;
	}
	return (*tokens);
}

size_t		token_count(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens)
	{
		//ft_printf("token count [%p]\n", tokens);
		count++;
		tokens = tokens->next;
	}
	return (count);
}
