#include <builtins.h>

t_builtin			builtin_get(const char *name)
{
	const char		*names[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	const int		lengths[] = {4, 3, 4, 7, 5, 4, 5};
	const t_builtin builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, &ft_unset, &ft_env, &ft_exit};
	int				i;

	i = 0;
	while (i < 7 && ft_strncmp(name, names[i], lengths[i]))
		i++;
	return ((i < 7) ? builtins[i] : NULL);
}
