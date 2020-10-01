#ifndef TOKEN_H

# include <stdint.h>
# include <stdbool.h>

# define OPERATORS "|><;"

# define NONE					0
# define PIPE					1
# define REDIRECTION_GREATHER	2
# define REDIRECTION_LESSER		4
# define SEMICOLON				8
# define REDIRECTION_DGREATHER	16

typedef struct		s_token
{
	char			*data;
	t_operator_t		type;
	struct s_token	*next;
}					t_token;

t_token		*token_clear(t_token **tokens, bool free_data);
size_t		token_count(t_token *tokens);

t_token		*parse_token(const char **txt);


#endif
