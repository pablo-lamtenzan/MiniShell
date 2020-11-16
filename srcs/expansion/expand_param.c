#include <expansion.h>

/*
** Expand the ~, ~+ and ~- patterns using the environment.
**
** ~ -> $HOME if set or "" otherwise
** ~+ -> $PWD if set
** ~- -> $OLDPWD if set
**
** This function sets the input pointer past the last parsed character.
**
** returns the expanded value, or NULL if there is no expansion.
*/
static const char	*tilde_expand(const char **input, t_env *env)
{
	const char	*val;

	if (**input != '~')
		val = NULL;
	else if ((*input)[1] == '+')
	{
		if ((val = env_get(env, "PWD", 3)))
			(*input) += 2;
	}
	else if ((*input)[1] == '-')
	{
		if ((val = env_get(env, "OLDPWD", 6)))
			(*input) += 2;
	}
	else
	{
		if (!(val = env_get(env, "HOME", 4)))
			val = "";
		(*input)++;
	}
	return (val);
}

/*
** Expand the $[KEY] pattern to it's value using the environment.
**
** returns the expanded value, or NULL if there is no expansion.
*/
static const char	*var_expand(const char **input, t_env *env)
{
	const char	*val;
	size_t		key_len;

	val = NULL;
	if (**input == '$' && (key_len = env_key_len((*input) + 1)))
	{
		if (!(val = env_get(env, ++(*input), key_len)))
			val = "";
		ft_dprintf(2, "[EXP][VAR] key: %.*s, val: %s\n", (int)key_len, (*input), val);
		(*input) += key_len;
	}
	return (val);
}

/*
** Expand a string using tilde and variable expansion.
**
** returns the expanded content in a t_line if succesfull or NULL otherwise.
*/
static t_line		*string_expand(const char *input, t_env *env)
{
	t_line		*exp;
	const char	*i;
	const char	*n;
	const char	*val;

	if (!(exp = line_new(4)))
		return (NULL);
	i = input;
	while (*(n = i))
	{
		if (!((val = var_expand(&n, env)) || (val = tilde_expand(&n, env))))
			i++;
		else
		{
			if ((input != i && !line_insert(exp, exp->len, input, i - input))
			|| (*val && !line_insert(exp, exp->len, val, ft_strlen(val))))
			{
				line_clear(&exp);
				return (NULL);
			}
			input = n;
			i = n;
		}
	}
	if (input != i && !line_insert(exp, exp->len, input, i - input))
		line_clear(&exp);
	return (exp);
}

/*
** Expand a parameter's parts that are not single quoted.
**
** The expanded data is freed and replaced.
** returns true if successfull or false otherwise.
*/
bool				param_expand(t_tok *parts, t_env *env)
{
	t_line		*expanded;

	while (parts)
	{
		if (!(parts->type & TOK_SQUOTED))
		{
			if (!(expanded = string_expand(parts->data, env)))
				return (false);
			free(parts->data);
			parts->data = expanded->data;
			free(expanded);
		}
		parts = parts->next;
	}
	return (true);
}
