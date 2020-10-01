#ifndef CMD_H

# include <unistd.h>
# include <token.h>

typedef struct      s_cmd
{
    char            **av;
    int             ac;
    struct s_cmd    *next;
}                   t_cmd;

typedef uint8_t t_operator_t;

typedef struct      s_operator
{
    t_operator_t      type;
    struct s_cmd    *next;
}                   t_operator;

t_cmd           *cmd_clear(t_cmd *cmd_list);
t_cmd			*tokens_export(t_token *tokens);
t_cmd			*cmd_add(t_cmd **cmd_list, t_cmd *cmd);

#endif
