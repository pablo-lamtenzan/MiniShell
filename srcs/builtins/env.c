#include <minishell.h>

int	ft_env(t_map *env)
{
	if (env)
	{
		ft_env(env->next);
		ft_printf("%s=%s\n", env->key, env->value);
	}
	return (0);
}