#include <env.h>

/*
** returns the length of an assignment's key
*/
size_t	env_key_len(const char *key)
{
	const char	*i;

	i = key;
	while(ft_isalnum(*i) || *i == '_' || *i == '-')
		i++;
	return (i - key);
}

/*
** Free the allocated environment.
*/
void	env_clr(t_env **env)
{
	t_env	*current;

	//int i = 0;
	//ft_printf("env: %p\n", *env);
	while ((current = *env))
	{
		//ft_printf("TEST: %d\n", i++);
		*env = current->next;
		free(current->key);
		free(current);
	}
}

/*
** Allocates a new env node
*/
t_env	*env_new(char *key, bool exported, size_t key_len)
{
	t_env *new;

	if ((new = malloc(sizeof(t_env))))
	{
		new->exported = exported;
		new->key = ft_strdup(key);
		new->key_length = key_len;
		new->next = NULL;
	}
	return (new ? new : NULL);
}

/*
** Return the number of nodes in env
*/
size_t	env_size(t_env *env)
{
	size_t	size;
	t_env	*cp;

	size = 0;
	cp = env;
	while (cp && (cp = cp->next))
		size++;
	return (size);
}

/*
** Add back a node in the target env lnk-list
*/
bool	env_add_back(t_env **env, t_env *add)
{
	t_env	*rmb;

	if (!add)
		return (false);
	if (!env || !*env)
		*env = add;
	else
	{
		rmb = *env;
		while ((*env)->next)
			*env = (*env)->next;
		(*env)->next = add;
		*env = rmb;
	}
	return (true);		
}

/*
** Dup the current env and return a pointer to the first elem
*/
t_env	*env_dup(t_env *env)
{
	t_env	*cp;
	t_env	*dup;

	cp = env;
	dup = NULL;
	while (cp)
	{
		if (!(env_add_back(&dup, env_new(cp->key, cp->exported, cp->key_length))))
			return (NULL);
		cp = cp->next;
	}
	return (dup);
}
