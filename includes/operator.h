#ifndef OPERATOR_H
# define OPERATOR_H

# include <stdlib.h>

# define OPERATORS "|><;"

# define NONE       0
# define PIPE       1
# define REDIR_GR   2
# define REDIR_LE   4
# define REDIR_DG   8
# define SEMICOLON  16

typedef uint8_t t_operator_t;

typedef struct      s_operator
{
    t_operator_t        type;
    struct s_operator   *next;
}                   t_operator;

t_operator  *operator_new(t_operator_t type);
t_operator  *operator_add(t_operator **operators, t_operator *new);
t_operator  *operator_clear(t_operator **operators);

#endif
