#include <execution.h>

int	ft_unset(t_exec *args)
{
	while (args->ac-- > 1)
	{
		/* if (!key_check(av[ac]))
		{
			ft_printf("%s: %s `%s' : not a valid identifier\n",
				t->name, av[0], av[ac]);
			return (1);
		} */
		//t->env = map_del(t->env, args->av[args->ac]); // this line only was the 04/11/2020
		//glob_env = map_del(cmd->glob_env, cmd->av[cmd->ac]);
	}
	return (SUCCESS);
}
