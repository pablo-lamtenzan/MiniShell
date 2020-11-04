#ifndef TOKEN_H
# define TOKEN_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>

typedef uint8_t		t_token_t;

typedef struct		s_token
{
	char			*data;
	t_token_t		type;
	struct s_token	*next;
}					t_token;

t_token		*token_new(char *data, t_token_t type);
t_token		*token_add(t_token **tokens, t_token *new);

t_token		*token_clear(t_token **tokens, bool free_data);
size_t		token_count(t_token *tokens);

t_token		*parse_token(const char **txt);

#endif
