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

	while ((current = *env))
	{
		*env = current->next;
		free(current->key);
		free(current);
	}
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
** Return a copy of parent env
*/
t_env	*env_dup(t_env *curr)
{
	t_env	*new;
	t_env	*cp;
	t_env	*cp_curr;
	size_t	size;

	if (!(new = malloc(sizeof(t_env) * (size = env_size(curr)))))
		return (NULL);
	cp = new;
	cp_curr = curr;
	while (cp)
	{
		cp->next = cp + sizeof(t_env);
		cp->key = ft_strdup(cp_curr->key);
		cp->exported = cp_curr->exported;
		cp->key_length = cp_curr->key_length;
		cp_curr = cp_curr->next;
		if (!cp_curr && !(cp->next = NULL))
			break ;
	}
	return (new);
}