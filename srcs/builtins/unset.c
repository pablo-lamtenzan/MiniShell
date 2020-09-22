#include "builtins.h"

int	ft_unset(int ac, char **av, t_data *t)
{
	// TODO: glob env
	while (ac-- > 1)
	{
		/* if (!key_check(av[ac]))
		{
			ft_printf("%s: %s `%s' : not a valid identifier\n",
				t->name, av[0], av[ac]);
			return (1);
		} */
		data->envp = map_del(data->envp, av[ac]);
		//glob_env = map_del(cmd->glob_env, cmd->av[cmd->ac]);
	}
	return (0);

}
