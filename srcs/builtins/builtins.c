/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/07 01:51:44 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/02 15:56:34 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>

t_executable	builtin_get(const char *name)
{
	static const t_builtin	map[] = {
		{"cd", &b_cd},
		{"echo", &b_echo},
		{"env", &b_env},
		{"exit", &b_exit},
		{"export", &b_export},
		{"pwd", &b_pwd},
		{"unset", &b_unset},
		{"fg", &b_fg},
		{"bg", &b_bg},
		{"kill", &b_kill},
		{"jobs", &b_jobs},
		{"disown", &b_disown},
		{"wait", &b_wait},
		{"history", &b_history}
	};
	size_t					i;

	i = 0;
	while (i < sizeof(map) / sizeof(*map)
	&& ft_strcmp(name, map[i].name))
		i++;
	return ((i < sizeof(map) / sizeof(*map)) ? map[i].exec : NULL);
}
