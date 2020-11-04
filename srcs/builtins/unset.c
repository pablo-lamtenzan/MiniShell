#include <execution.h>

int	ft_unset(t_exec *args, t_term *t)
{
	// TODO: glob env
	while (args->ac-- > 1)
	{
		/* if (!key_check(av[ac]))
		{
			ft_printf("%s: %s `%s' : not a valid identifier\n",
				t->name, av[0], av[ac]);
			return (1);
		} */
		t->env = map_del(t->env, args->av[args->ac]);
		//glob_env = map_del(cmd->glob_env, cmd->av[cmd->ac]);
	}
	return (0);

}
