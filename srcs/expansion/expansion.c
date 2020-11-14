#include <expansion.h>


char			*param_export(t_tok *param)
{
	t_tok		*tokens;
	char		*result;
	size_t		size;

	tokens = param;
	size = 0;
	while (tokens)
	{
		size += ft_strlen(tokens->data);
		tokens = tokens->next;
	}
	size++;
	if (!(result = malloc(sizeof(*result) * size)))
		return (NULL);
	*result = '\0';
	while ((tokens = param))
	{
		param = param->next;
		ft_strlcat(result, tokens->data, size);
		free(tokens->data);
		free(tokens);
	}
//	ft_dprintf(2, "[EXP][EXPORT] result: '%s'\n", result);
	return (result);
}

char *const		*token_export(t_tok *tokens, int *ac)
{
	char		**av;
	t_tok		*next;
	size_t		i;

	if (!(av = malloc(sizeof(*av) * (token_len(tokens) + 1))))
	{
		token_clr(&tokens);
		return (NULL);
	}
	i = 0;
	while (tokens)
	{
		next = tokens->next;
		av[i++] = tokens->data;
		free(tokens);
		tokens = next;
	}
	av[i] = NULL;
	*ac = (int)i;
	return (av);
}

static bool		var_assign(t_tok **param, t_env **env)
{
	char	status;
	t_tok	*curr;

//	ft_dprintf(2, "[EXP][VAR][ASSIGN] Input: '%s'\n", (*param)->data);
	status = 1;
	while (status == 1 && (curr = *param))
	{
		if ((status = env_assign(env, curr->data, false)) == 1)
		{
			(*param) = (*param)->next;
//			ft_dprintf(2, "[EXP][VAR][ASSIGN] assigned '%s'!\n", curr->data);
			free(curr->data);
			free(curr);
		}
	}
	return (status != -1);
}

static bool		param_split(t_tok *params)
{
	char	*exported;

	while (params)
	{
		if (!(exported = param_export(params->data)))
			return (false);
		params->data = exported;
		params = params->next;
	}
	return (true);
}

static t_tok	*param_expand(t_tok *param, t_env *env)
{
	const char	*data;
	const char	*key;
	size_t		key_len;
	const char	*val;
	t_tok		*expanded;
	t_tok		*token;

	expanded = NULL;
	if (!(token = token_new(NULL, TOK_PARAM)))
	{
		token_clr(&expanded);
		return (NULL);
	}
//	ft_dprintf(2, "[NEW] for: '%s'\n", param->data);
	token_add_back(&expanded, token);
	while (param)
	{
		data = param->data;
		while (*(key = data))
		{
			key_len = 0;
			while (*key
			&& (*key != ENV_OP_VAR || !(key_len = env_key_len(key + 1))))
				key++;
			if (key != data)
			{
//				ft_dprintf(2, "[EXP][PRM] simple: '%.*s'\n", (int)(key - data), data);
				if (!(token = token_strndup(data, key - data, param->type)))
				{
					token_clr(&expanded);
					return (NULL);
				}
//				ft_dprintf(2, "[ADD]: '%s'\n", token->data);
				token_add_back((t_tok**)&expanded->data, token);
				data = key;
			}
			if (key_len)
			{
				key++;
				val = env_get(env, key, key_len);
//				ft_dprintf(2, "[EXP][PRM] var: '%.*s', val: '%s'\n", (int)key_len, key, val);
				if (!(token = token_strndup(val, ft_strlen(val), TOK_PARAM)))
				{
					token_clr(&expanded);
					return (NULL);
				}
//				ft_dprintf(2, "[ADD]: '%s'\n", token->data);
				token_add_back((t_tok**)&expanded->data, token);
				data = key + key_len;
//				ft_dprintf(2, "[EXP][PRM] key is at: '%s'\n", key);
			}
		}
		param = param->next;
	}
	return (expanded);
}

/*
** Expand a parameter list into a NULL-terminated c-string array.
**
** returns the expanded array if successfull, or NULL otherwise.
*/
char *const		*token_expand(t_tok *cmd, t_env **env, int *ac)
{
	t_tok	*expanded;
	t_tok	*token;

	*ac = 0;
	expanded = NULL;
//	ft_dprintf(2, "[EXP] Start, cmd: %p\n", cmd);
	while (cmd)
	{
//		ft_dprintf(2, "[EXP][CMD] type: %hu, data: %p\n", cmd->type, cmd->data);
		if (cmd->type & TOK_PARAM && !(token = param_expand(cmd->data, *env)))
		{
			token_clr(&expanded);
			return (NULL);
		}
		token_add_back(&expanded, token);
		cmd = cmd->next;
	}
	if (!expanded)
		return (NULL);
	if (!param_split(expanded) || !var_assign(&expanded, env))
	{
		token_clr(&expanded);
		return (NULL);
	}
	return (token_export(expanded, ac));
}
