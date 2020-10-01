#include <cmd.h>

t_cmd			*cmd_clear(t_cmd **cmd_list)
{
	t_cmd	*curr;

	while (curr = *cmd_list)
	{
		*cmd_list = curr->next;
		free(curr);
	}
	return (curr);
}

t_cmd			*tokens_export(t_token *tokens)
{
	const size_t	count = token_count(tokens);
	t_cmd			*cmd;
	t_token			*curr;

	if (cmd = malloc(sizeof(*cmd)))
	{
		cmd->ac = 0;
		if (cmd->av = malloc(sizeof(*cmd->av) * (count + 1)))
		{
			curr = tokens;
			while (curr)
			{
				cmd->av[cmd->ac++] = curr->data;
				curr = curr->next;
			}
			cmd->av[cmd->ac] = NULL;
		}
		free(cmd);
	}
	token_clear(&tokens, true);
	return (NULL);
}

t_cmd			*cmd_add(t_cmd **cmd_list, t_cmd *cmd)
{
	if (!cmd_list)
		return (NULL);
	if (*cmd_list)
		(*cmd_list)->next = cmd;
	else
		*cmd_list = cmd;
	return (cmd);
}
