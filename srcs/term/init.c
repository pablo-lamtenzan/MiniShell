/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 13:20:36 by chamada           #+#    #+#             */
/*   Updated: 2020/12/11 17:20:21 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

void		term_perror(t_term_err err)
{
	static const char	*msgs[] = {
		"Read error",
		"Write error",
		"Allocation error",
		"Failed to load tty entry",
		"Failed to load tty attributes",
		"Failed to set tty attributes",
	};
	unsigned int		i;

	if (err < 0 && (i = -err - 1) < sizeof(msgs) / sizeof(*msgs))
		ft_dprintf(STDERR_FILENO, "%s: %s"TERM_ENDL,
			msgs[i], strerror(errno));
}

t_term_err	term_init_env(t_env **env, const char *cwd)
{
	static const char	*ps[2] = {
		C_D"["S_B""C_RI"$USER"C_YI"@"C_GI"$DIRNAME"S_D""C_D"]$ ",
		"> ",
	};
	char *const			dirname = ft_basename(cwd);
	bool				success;

	success = dirname && env_set(env, "DIRNAME", dirname, false)
		&& env_set(env, "PS1", ps[0], false)
		&& env_set(env, "PS2", ps[1], false);
	free(dirname);
	return ((success) ? TERM_EOK : TERM_EALLOC);
}
