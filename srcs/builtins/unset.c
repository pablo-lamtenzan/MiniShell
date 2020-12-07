#include <execution.h>

int	b_unset_old(t_exec *args)
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


int	b_unset(t_exec *args)
{
	t_env	*next;
	t_env	*first;
	t_env	*prev;

	prev = NULL;
	first = *args->env;
	while (args->ac-- > 1)
	{
		while (*args->env)
		{
			next = (*args->env)->next;
			if (!ft_strncmp(args->av[args->ac], (*args->env)->key, \
					ft_strlen(args->av[args->ac])))
			{
				if (first && first == *args->env)
					first = first->next;
				free(*args->env);
				if (prev)
					prev->next = next;
			}
			else
				prev = *args->env;
			*args->env = next;
			ft_dprintf(2, "Env addr: %p\n", *args->env);
		}
		*args->env = first;
	}
	return (SUCCESS);
}
