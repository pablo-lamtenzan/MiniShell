#include <expansion.h>

/*
** Split a string using IFS if it is set, or whitespace otherwise.
**
** returns true if successfull or false otherwise.
*/
static bool	str_split(t_line **curr_word, const char *data)
{
	const char	*i;
	const char	*start;

	i = data; // start at data
	while (*(start = i))
	{
		while (ft_isspace(*start))
			start++; // skip spaces
		if (i != start && (*curr_word)->len) // if there were spaces, create new word if there is one already
		{
			if (!((*curr_word)->next = line_new(1)))
				return (false);
			*curr_word = (*curr_word)->next;
			i = start; // start after the spaces
		}
		while (*i && !ft_isspace(*i)) // go to next space
			i++;
		if (i != start
		&& !line_insert(*curr_word, (*curr_word)->len, start, i - start)) // if there is any content
			return (false);
	}
	return (true);
}

/*
** Split a parameters unquoted parts into separate words.
**
** The parameters parts are freed, and the list pointer is set to NULL.
**
** returns a list of separated words if successfull, or NULL otherwise.
*/
static t_line	*param_split(t_tok **parts)
{
	t_tok		*part;
	t_line		*words;
	t_line		*curr;

	if (!(words = line_new(1)))
		return (NULL);
	curr = words;
	while ((part = (*parts)))
	{
		*parts = (*parts)->next;
		if ((part->type & TOK_MQUOTED)
		? !line_puts(curr, part->data) : !str_split(&curr, part->data))
		{
			lines_clear(&words);
			token_clr(&part);
			return (NULL);
		}
		free(part->data); // free popped part
		free(part);
	}
	return (words);
}

/*
** Split a parameter's list into separate words.
**
** The parameter's are freed, and the list pointer is set to NULL.
**
** returns a list of separated words if successfull, or NULL otherwise.
*/
t_line			*word_split(t_tok **params)
{
	t_line	*words;
	t_line	*new_words;
	t_tok	*param;

	words = NULL;
	while ((param = *params))
	{
		*params = (*params)->next;
		if (!(new_words = param_split((t_tok **)&param->data))) // split param parts into new words
		{
			token_clr(&param);
			line_clear(&words);
			return (NULL);
		}
		line_add_back(&words, new_words); // add new words to word list
		free(param); // free popped param
	}
	return (words);
}
