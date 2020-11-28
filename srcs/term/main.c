#include <term/term.h>
#include <expansion.h>

void	env_print(t_env *env)
{
	while (env)
	{
		ft_dprintf(2, "%s\n", env->key);
		env = env->next;
	}
}

void	on_winch(int sig)
{
	ft_dprintf(2, "WINCH(%d)!", sig);
}

int main(int ac, const char **av, const char **ep)
{
//	const char	*name = av[0];
	t_env		*env;
	t_term_err	status;

	(void)ac;
	(void)av;
	// TODO: Init env and session
	if (!(env = env_import(ep)))
		return (1);
	if (!term_init(&env))
	{
		env_clr(&env);
		return (1);
	}
	signal(SIGWINCH, &on_winch);
	status = TERM_EOK;
	while ((g_term.msg = string_expand(TERM_PS1, env))
	&& (status = term_prompt(NULL)) == TERM_ENL)
		free(g_term.msg);
	term_destroy();
	env_clr(&env);
	if (g_term.is_interactive)
		ft_dprintf(2, "exit\n");
	return (status);
}
