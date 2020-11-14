#ifndef EXPANSION_H
# define EXPANSION_H

# include <term/token.h>
# include <term/env.h>

# define ENV_OP_VAR	'$'

char *const	*token_expand(t_tok *tokens, t_env **env, int *ac);

#endif
