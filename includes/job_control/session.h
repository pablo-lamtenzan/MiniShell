/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:32:24 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:53:03 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSION_H
# define SESSION_H

/*
** External
*/
# include <sys/param.h>

/*
** Local
*/
# include <job_control/zombies.h>
# include <job_control/history.h>
# include <env.h>

/*
** Flags
*/
# define OPEN_PRINT		1
# define PIPED_CMD		2
# define BUILTIN		4

typedef struct			s_session
{
	int					st;
	t_group				*groups;
	t_group				*nil;
	t_history			*hist;
	t_background		*zombies;
	t_deadzombie		*dead_zombies;
	char				**input_line;
	size_t				input_line_index;
	unsigned char		exit_count;
	char				cwd[PATH_MAX + 1];
	char				*name;
	t_env				*env;
	char				flags;
}						t_session;

t_session				g_session;

t_session				*session_start(t_session *session,
	const char *name, const char **envp);
void					session_end(t_session *session);
bool					session_empty();
t_session				*session_dup();
void					session_destroy(t_session **target);

#endif
