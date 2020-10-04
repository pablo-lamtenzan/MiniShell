#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <libft.h>
# include <token.h>
# include <cmd.h>

# define TOKEN      0
# define S_QUOTE	1
# define D_QUOTE	2
# define B_SLASH	4
# define OP         8
# define SEMICOL    16

# define EMPTY      0
# define END        1
# define CONTINUE   2
# define ERROR      -1

int lexer_tokenize(const char **input, t_cmd **cmds, t_operator **operators);

#endif
