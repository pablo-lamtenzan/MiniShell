
/* Just temporally defines */

# define END 1 // end of input line
# define WAIT 2 // requires more input

t_tok				*input_line_concatenate(t_input_line *lines);

/*
** Used at the end of the lexer (for each lexed line)
** Returns NULL if malloc error
** Else return a pointer to the input tokens
** Always return a pointer to the tokens (have to use the tokens after this fct when the status is END)
** Arg "lines" could be member of a struct and be started to null (no have to think about it)
**	it will be allocated and freed automatically
*/

t_tok				*input_line_update(t_input_line *lines, t_lex_st *st)
{
	t_tok*			tokens;

	/* Add to the queue the line */
	if (!input_line_push_back(&lines, input_line_new(st)))
		return (NULL);

	tokens = NULL;
	/* The input line is not multiline or not multiline anymore */
	if (st->wait & END)
	{
		/* Concatenate the tokens and return them */
		tokens = input_line_concatenate(lines);
		/* Empty the queue */
		input_line_destroy(lines);
	}
	/* The input is multiline */
	else
	{
		/* Return the first token of the first line 
		This is just for no return NULL
		In this case the tokens must not be used
		*/
		tokens = lines->lexed_lines->tokens;
	}
	return (tokens);
}

/* The concatenation happends here */
t_tok				*input_line_concatenate(t_input_line *lines)
{
	t_tok			*tokens;
	t_input_line	*first;

	tokens = NULL;
	first = lines;
	while (lines)
	{
		/* Concatenate */
		if (tokens)
			tokens->next = lines->lexed_lines->tokens;

		/* Get the last token of each line */
		tokens = lines->lexed_lines->tokens;
		while (tokens && tokens->next)
			tokens = tokens->next;

		/* Iterate */
		lines = lines->next;
	}
	/* And as easy as that we have the multiline tokens in same format than before */
	lines = first;
	return (lines->lexed_lines->tokens);
}

/* Used for update the current line, must be called at the begin before starting the lexer */
void				lex_init(t_input_line *lines, t_lex_st *st)
{
	/* No multiline */
	if (!lines)
		return ;

	/* Go to the last line (the prevous line) */
	while (lines->next)
		lines = lines->next;
	
	/* Update */
	st->wait = lines->lexed_lines->wait;
	st->subshell_level = lines->lexed_lines->subshell_level;
}

/*
** JUST A LITTLE CONCLUSION:
**
** 1) Use lex_init before start lexing it will update the st if needed
** 2) Must have correctly set the flags END and WAIT (their current version)
** 3) At the end of each lexer call input_line_update at automatilly will create or destroy ressources
** 4) Must have a reference to input_line and init it to NULL
** 5) History is not handled in this example but is the same
**		just have to concatenate the "input"
** 6) The engine is just a queue who stores the t_lex_st when multiline is all or else empty itself
*/
