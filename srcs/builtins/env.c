#include <execution.h>

void	env_print(int fd, t_map *env)
{
	if (env)
	{
		env_print(fd, env->next);
		ft_dprintf(fd, "%s=%s\n", env->key, env->value);
	}
}

int		ft_env(t_exec *args, t_term *t)
{
	t_env* curr;

	curr = t->env;
	(void) args;
	while (curr)
	{
		ft_putstr_fd(curr->key, args->fds[1]);
		curr = curr->next;
	}
	//env_print(args->fds[1], t->env);
	return (0);
}
