#include <token.h>

t_token		*token_new(char *data, t_token_t type)
{
	t_token	*new;

	if (!data || !(new = malloc(sizeof(*new))))
	{
		// TODO: Free data on error
		return (NULL);
	}
	new->data = data;
	new->type = type;
	new->next = NULL;
	return (new);
}

t_token		*token_add(t_token **tokens, t_token *new)
{
	t_token	*curr;

	if (!new)
		return (NULL);
	new->next = NULL;
	if (*tokens)
	{
		curr = *tokens;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	else
		*tokens = new;
	return (new);
}

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
