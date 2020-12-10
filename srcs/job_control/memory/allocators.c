/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocators.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:22:03 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:45:13 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <libft.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
** Job control objects allocation and initialisation functions
*/

t_session			*session_start(t_session *sess,
	const char *name, const char **ep)
{
	t_group *const	nil = ft_calloc(1, sizeof(*nil));
	char *const		basename = ft_basename(name);
	t_env			*env;

	env = NULL;
	if (nil && basename && (env = env_import(ep))
	&& (sess || (sess = malloc(sizeof(*sess)))))
	{
		ft_bzero(sess, sizeof(*sess));
		if (getcwd(sess->cwd, PATH_MAX))
		{
			nil->prev = nil;
			nil->next = nil;
			sess->nil = nil;
			sess->groups = nil;
			sess->name = basename;
			sess->env = env;
			return (sess);
		}
		ft_dprintf(2, "getcwd: %s\n", strerror(errno));
	}
	free(nil);
	free(basename);
	env_clr(&env);
	return (NULL);
}

t_group				*group_new(void)
{
	t_group			*group;

	if (!(group = ft_calloc(1, sizeof(t_group))))
		return (NULL);
	if (!(group->nil = ft_calloc(1, sizeof(t_process))))
	{
		free(group);
		return (NULL);
	}
	group->nil->next = group->nil;
	group->nil->prev = group->nil;
	group->active_processes = group->nil;
	return (group);
}

t_process			*process_new(pid_t pid, int wstatus, char*const *data)
{
	t_process		*process;

	if (!(process = ft_calloc(1, sizeof(t_process))))
		return (NULL);
	*process = (t_process){.pid=pid, .wstatus=wstatus, .data=data, .ret=-1};
	return (process);
}

t_deadzombie		*deadzombie_new(t_process *target)
{
	t_deadzombie	*endzombie;

	if (!(endzombie = ft_calloc(1, sizeof(t_deadzombie))))
		return (NULL);
	*endzombie = (t_deadzombie){.deadzombie=target};
	return (endzombie);
}
