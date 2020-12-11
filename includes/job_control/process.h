/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:27:35 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:52:59 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

/*
** External
*/
# include <unistd.h>
# include <stdbool.h>

/*
** Flags
*/
# define BACKGROUND		1
# define STOPPED		2
# define EXITED			4
# define SIGNALED		8
# define KILLED			16
# define RESTRICT_OP	32
# define NO_HANGUP		64
# define NO_DELETE		128

typedef struct			s_process
{
	pid_t				pid;
	int					wstatus;
	unsigned char		flags;
	char*const			*data;
	struct s_process	*next;
	struct s_process	*prev;
	int					ret;
}						t_process;

t_process				*process_new(pid_t pid, int wstatus, char*const *data);
void					process_insert(t_process *prev, t_process *next,
						t_process *target);
void					process_remove(t_process **target);
bool					is_leader(t_process *target);

#endif
