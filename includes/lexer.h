#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <libft.h>
# include <token.h>
# include <cmd.h>

# define TOKEN      1
# define SPACE      2
# define S_QUOTE	4
# define D_QUOTE	8
# define B_SLASH	16
# define OP         32
# define SEMICOL    64

# define EMPTY      0
# define END        1
# define CONTINUE   2
# define ERROR      -1

int lexer_tokenize(char **input, t_cmd **cmds, t_operator **operators);

#endif
