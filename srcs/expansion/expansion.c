/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 01:31:51 by chamada           #+#    #+#             */
/*   Updated: 2020/12/12 20:59:15 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <expansion.h>

static char	*param_export(t_tok *param, bool free_params)
{
	t_line	*line;
	char	*arg;
	t_tok	*curr;

	if (!(line = line_new(1)))
		return (NULL);
	while ((curr = param))
	{
		param = param->next;
		if (!line_insert(line, line->len, curr->data, ft_strlen(curr->data)))
		{
			if (free_params)
				token_clr(&curr);
			return (line_clear(&line));
		}
		if (free_params)
		{
			free(curr->data);
			free(curr);
		}
	}
	arg = line->data;
	free(line);
	return (arg);
}

/*
** Assign variables to the environment.
**
** Assignments are composed of unquoted keys, followed by an equal sign and the
** assigned value.
**
** Assignation stops on the first invalid assignment, or on environment errors.
**
** returns true if successfull or false otherwise.
*/

static bool	var_assign(t_tok **params, t_env **env)
{
	char	status;
	t_tok	*param;
	t_tok	*part;
	size_t	key_len;
	char	*assignment;

	status = 1;
	while (status == 1 && (param = *params)
	&& (part = param->data) && !(part->type & (TOK_MQUOTED))
	&& (key_len = env_key_len(part->data, true))
	&& (((char*)part->data)[key_len] == '='))
	{
		if (!(assignment = param_export(part, 0)))
			return (false);
		if ((status = env_assign(env, assignment, true, true)) == 1)
		{
			(*params) = (*params)->next;
			token_clr((t_tok**)&param->data);
			free(param);
		}
		free(assignment);
	}
	return (status != -1);
}

/*
** Export a word-list to a NULL-terminated c-string array.
**
** This function clears the input word-list.
**
** returns a pointer to the array if successfull, or NULL otherwise.
*/

static char	**args_export(t_line **words, int *ac)
{
	const size_t	length = line_count(*words);
	char			**av;
	t_line			*curr;

	*ac = 0;
	if (!(av = malloc(sizeof(*av) * (length + 1))))
	{
		lines_clear(words);
		return (NULL);
	}
	while ((curr = *words))
	{
		*words = (*words)->next;
		av[(*ac)++] = curr->data;
		free(curr);
	}
	av[*ac] = NULL;
	return (av);
}

/*
**	Expand command tokens into a NULL-terminated c-string array.
**
**	Variables are evaluated and assigned using the environment.
**	returns the allocated c-string array if successfull, or NULL otherwise
*/

char		**tokens_expand(t_tok **params, t_env **env, int *ac)
{
	t_tok	*param;
	t_line	*args;

	args = NULL;
	param = *params;
	while (param && param_expand(param->data, *env))
		param = param->next;
	if (param
	|| !var_assign(params, env) || (*params && !(args = word_split(params))))
	{
		token_clr(params);
		return (NULL);
	}
	return (args_export(&args, ac));
}
