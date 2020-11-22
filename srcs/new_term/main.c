#include "term.h"

int main(int ac, const char **av, const char **ep)
{
//	const char	*name = av[0];
	t_env		*env;
	t_term		term;
	t_term_err	status;

	(void)ac;
	(void)av;
	// TODO: Init env and session
	if (!(env = env_import(ep)))
		return (1);
	if (!term_init(&term, &env))
	{
		env_clr(&env);
		return (1);
	}
	term.msg = TERM_PS1;
	status = TERM_EOK;
	while ((status = term_prompt(&term)) == TERM_ENL)
	{
		;
	}
	term_destroy(&term);
	env_clr(&env);
	if (term.is_interactive)
		ft_dprintf(2, "exit\n");
	return (status);
}
