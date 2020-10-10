#include <minishell.h>

void	env_print(int fd, t_map *env)
{
	if (env)
	{
		env_print(fd, env->next);
		ft_dprintf(fd, "%s=%s\n", env->key, env->value);
	}
}

int		ft_env(t_args *args, t_term *t)
{
	(void) args;
	env_print(1, t->env);
	return (0);
}
