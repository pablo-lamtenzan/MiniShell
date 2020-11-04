#include <cmd.h>

t_cmd			*cmd_clear(t_cmd **cmd_list)
{
	t_cmd	*curr;

	while ((curr = *cmd_list))
	{
		*cmd_list = curr->next;
		free(curr);
	}
	return (NULL);
}

t_cmd			*tokens_export(t_token *tokens)
{
	const size_t	count = token_count(tokens);
	t_cmd			*cmd;
	t_token			*curr;

	if ((cmd = malloc(sizeof(*cmd))))
	{
		cmd->ac = 0;
		if ((cmd->av = malloc(sizeof(*cmd->av) * (count + 1))))
		{
			curr = tokens;
			while (curr)
			{
				ft_dprintf(2, "[lexer][cmd][av][%d] '%s'\n", cmd->ac, curr->data);
				cmd->av[cmd->ac++] = curr->data;
				curr = curr->next;
			}
			cmd->av[cmd->ac] = NULL;
			cmd->next = NULL;
		}
		else
		{
			free(cmd);
			cmd = NULL;
		}
		token_clear(&tokens, false);
	}
	return (cmd);
}

t_cmd			*cmd_add(t_cmd **cmds, t_cmd *cmd)
{
	t_cmd	*curr;

	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	if (*cmds)
	{
		curr = *cmds;
		while (curr->next)
			curr = curr->next;
		curr->next = cmd;
	}
	else
		*cmds = cmd;
	return (cmd);
}
