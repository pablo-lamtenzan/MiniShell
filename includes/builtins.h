/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 10:37:14 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 22:03:32 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/*
**	External
*/
# include <limits.h>

/*
**	Local
*/
# include <term/term.h>
# include <execution.h>

typedef	struct			s_builtin
{
	const char			*name;
	t_executable		exec;
}						t_builtin;

/*
**	Subject builtins
*/
int						b_cd(t_exec *args);
int						b_echo(t_exec *args);
int						b_env(t_exec *args);
int						b_exit(t_exec *args);
int						b_export(t_exec *args);
int						b_pwd(t_exec *args);
int						b_unset(t_exec *args);

/*
**  Job control builtins
*/
int						b_fg(t_exec *args);
int						b_bg(t_exec *args);
int						b_kill(t_exec *args);
int						b_jobs(t_exec *args);
int						b_disown(t_exec *args);
int						b_wait(t_exec *args);

/*
**	Other builtins
*/
int						b_history(t_exec *args);

t_executable			builtin_get(const char *name);

#endif
