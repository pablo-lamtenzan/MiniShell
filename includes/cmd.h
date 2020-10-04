#ifndef CMD_H
# define CMD_H

# include <unistd.h>
# include <token.h>
# include <operator.h>

typedef struct      s_cmd
{
	char*		*av;
	int				ac;
	struct s_cmd	*next;
}					t_cmd;

t_cmd           *cmd_clear(t_cmd **cmd_list);
t_cmd			*tokens_export(t_token *tokens);
t_cmd			*cmd_add(t_cmd **cmd_list, t_cmd *cmd);

#endif
