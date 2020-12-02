#include <lexer/lexer.h>

/*
** SCOPE_IN
** Opening parenthesis.
**
** '('
*/
static t_lex_err	lex_scope_in(t_lex_st *st)
{
	t_tok	*scope_in;

	if (*st->input == '(')
	{
//		ft_dprintf(2, "[LEX][SCOPE][ IN] Input: '%s'\n", st->input);
//		ft_dprintf(2, "[LEX][SCOPE][ IN] MATCH!\n");
		st->input++;
		st->wait |= TOK_SCOPE_OUT;
		st->subshell_level++;
		if (!(scope_in = token_new(NULL, TOK_SCOPE_IN)))
			return (LEX_EALLOC);
		token_add_back(&st->tokens, scope_in);
		return (LEX_EOK);
	}
	return (LEX_ENOMATCH);
}

/*
** SCOPE_OUT
** Closing parenthesis.
**
** ')'
*/
static t_lex_err	lex_scope_out(t_lex_st *st)
{
	t_tok	*scope_out;

//	ft_dprintf(2, "[LEX][SCOPE][OUT] Input: '%s'\n", st->input);
	if (*st->input == ')')
	{
//		ft_dprintf(2, "[LEX][SCOPE][OUT] MATCH!\n");
		if (st->subshell_level == 0)
			return (LEX_ESYNTAX);
		st->wait &= ~TOK_SEP;
		st->input++;
		st->subshell_level--;
		if (st->subshell_level == 0)
			st->wait &= ~TOK_SCOPE_OUT;
		if (!(scope_out = token_new(NULL, TOK_SCOPE_OUT)))
			return (LEX_EALLOC);
		token_add_back(&st->tokens, scope_out);
		return (LEX_EOK);
	}
	return (LEX_ENOMATCH);
}

/*
** SUBSHELL
** TOKENS wrapped in parenthesis.
**
** SCOPE_IN TOKENS SCOPE_OUT
*/
t_lex_err	lex_subshell(t_lex_st *st)
{
	t_lex_err	status;

//	ft_dprintf(2, "[LEX][SUBSH] Input: '%s'\n", st->input);
	// TODO: Fix scope resume
	if ((status = lex_scope_in(st)) == LEX_EOK)
	{
//		ft_dprintf(2, "[LEX][SUBSH][ IN] Status: %d, Wait: '%d'\n", status, st->wait);
		if ((status = lex_tokens(st)) == LEX_EOK
		|| (status == LEX_EWAIT && st->wait & (TOK_SCOPE_OUT | TOK_SEP)))
		{
			status = lex_scope_out(st);
//			ft_dprintf(2, "[LEX][SUBSH][OUT] Status: %d Wait: '%d'\n", status, st->wait);
		}
//		ft_dprintf(2, "[LEX][SUBSH][END] Status '%d'\n", status);
	}
	return (status);
}

/* MULTINE IMPLEMENTATION */

/*
** NOTES:
** - When the multine is end the history remember to hole line (logical)
** - It seems any level of subshells can be reached in multine format in bash
** - Only seems to work with conditional separators or pipe operator
**
** IMPLEMENTATION IDEA
** - If theres a multiline operator/separator put a flag
** - If after this operator theres something remove the flag
** - If starting a new lexer operation (new line lexed) the flag is on
**		minish must require more input in a new line and block the execution
** - Need to remember all the lexer flags/status of the previous lines
** - So, need just a kind a linked-list who stores the t_lex_st
**		- each operation, a node is created and keeped or deleted if theres no multiline
**		- if there's multiline the nodes are keeped and added like a queue, yeah is that what i need: a queue
**		- just need to use the nodes to execute:
**			- 1) One node means a normal execution (and will be deleted just after send the tokens to the execution engine)
**			- 2) More than one is the same but have to concatenate all the tokens
*/

/* I WILL IMPLEMENT ALL IN THIS FILE FOR THE MOMMENT */

/* First of all we need a mini-lib to create-operate-destroy the queue */

/* Data storage */
typedef struct			s_input_line
{
	t_lex_st			*lexed_lines;
	struct s_input_line	*next;
}						t_input_line;

/* Creation */
t_input_line			*input_line_new(t_lex_st *input)
{
	t_input_line		*input_line;

	if (!(input_line = malloc(sizeof(t_input_line))))
		return (NULL);
	*input_line = (t_input_line){.lexed_lines=input, .next=NULL};
	return (input_line);
}

/* Destruction (destroy all always) */
void					input_line_destroy(t_input_line *head)
{
	t_input_line		*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

/* Adding a node, is bool because is implemented to do: "input_line_push_back(&input_line, input_line_new(st)); */
bool					input_line_push_back(t_input_line **lines, t_input_line *new_line)
{
	t_input_line		*first;

	if (!new_line)
		return (false);
	first = *lines;
	/* At least one input line in the queue */
	if (*lines)
	{
		while ((*lines)->next)
			*lines = (*lines)->next;
		(*lines)->next = new_line;
		*lines = first;
	}
	/* Queue is empty */
	else
		*lines = new_line;
	return (true);
}

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