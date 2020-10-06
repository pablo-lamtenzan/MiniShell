#include <libft.h>

#include <stdbool.h>

typedef struct	s_index {
	bool			export;
	char			**data;
	size_t			key_length;
	size_t			size;
	struct s_index	*next;
}				t_index;

typedef struct	s_env {
	t_index	*index;
	size_t	length;
	size_t	size;
	char	**envp;
}				t_env;

t_index	*index_get(t_index *index, const char *key)
{
	while (index && ft_strncmp(*index->data, key, index->key_length))
		index = index->next;
	return (index);
}

t_index	*index_add(t_index **index, const char **data, size_t data_size, bool export)
{
	t_index		*new;

	if ((new = malloc(sizeof(*new))))
	{
		if ((new->key_length = ft_strpos(*data, '=')) > 0)
		{
			new->export = export;
			new->data = *data;
			new->size = data_size;
			new->next = *index;
			*index = new;
		}
		else
		{
			free(new);
			new = NULL;
		}
	}
	return (new);
}

bool	env_load(t_env *env, const char **envp)
{
	const char	**it;

	it = envp;
	while (it++)
		;
	env->length = it - envp;
	if ((env->envp = malloc(sizeof(*env->envp) * (env->length + 1))))
	{
		it = env->envp;
		while (*envp)
		{
			*it = *envp++;
			index_add(&env->index, it++, 0, true);
		}
		*it = *envp;
	}
	return (env->envp);
}

t_env	*env_import(const char **envp)
{
	t_env	*new;
	
	if (new = malloc(sizeof(new)))
	{
		if (!env_load(new, envp))
		{
			free(new);
			new = NULL;
		}
	}
	return (new);
}

char	*env_eval(t_env *env, const char *assignment)
{
	
}

char	*env_set(t_env *env, const char *key, const char *value)
{
	const size_t	key_length = ft_strlen(key);
	const size_t	value_length = ft_strlen(value);
	const size_t	total_size = key_length + value_length + 2;
	t_index			*index;

	if ((index = index_get(env->index, key)))
	{
		if (total_size > index->size)
		{
			free(index->data);
			if ((*index->data = malloc(sizeof(**index->data) * total_size * 2)))
			{
				index->size = total_size * 2;
				ft_memcpy(*index->data, key, key_length);
				(*index->data)[key_length] = '=';
				ft_memcpy(*index->data + key_length + 1, value, value_length);
				(*index->data)[total_size - 1] = '\0';
			}
		}
		else
		{
			ft_memcpy(*index->data + key_length + 1, value, value_length);
			(*index->data)[total_size - 1] = '\0';
		}
	}
	else if ((index = malloc(sizeof(*index))))
	{
		index->
	}
}
