/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:08:59 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 17:38:34 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

bool			session_empty(void)
{
	return (g_session.nil->next == g_session.nil \
		&& g_session.nil->prev == g_session.nil);
}

t_session		*session_dup(void)
{
	t_session	*dup;

	if ((dup = ft_calloc(1, sizeof(t_session))) \
	&& (dup->name = ft_strdup(g_session.name)) \
	&& (dup->env = env_dup(g_session.env)) \
	&& ((dup->flags = g_session.flags) || !dup->flags)
	&& ((dup->exit_count = g_session.exit_count) || !dup->exit_count)
	&& (ft_strlcpy(dup->cwd, g_session.cwd, PATH_MAX)))
		return (dup);
	if (dup)
		session_destroy(&dup);
	return (dup);
}

void			session_destroy(t_session **target)
{
	env_clr(&(*target)->env);
	free((*target)->name);
	free(*target);
	*target = NULL;
}
