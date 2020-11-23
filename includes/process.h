/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 03:42:22 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>

# include <errors.h>

#define PROCESSES_MAX   4096
#define MANAGE          0

#define	BACKGROUD		1
#define STOPPED			2
#define EXITED			4
#define RESTRICT_OP		8
#define NO_HANGUP		16

typedef struct 			s_process
{
	pid_t				pid;
    int					wstatus;
	unsigned char		flags;
	char*const*			data;
	struct s_process*	next;
	struct s_process*	prev;
}						t_process;

typedef struct			s_group
{
	t_process*			active_processes;
	t_process*			nil;
	struct s_group*		next;
	struct s_group*		prev;
}						t_group;

typedef struct			s_session
{
	t_group*			groups; // all background processes by group
	t_process			processes[PROCESSES_MAX + 1]; // exec processes
	t_process			*history;
	t_group				*nil;
}						t_session;

// new stuff i wish is the last time i redo all

/*
** Session
*/
t_session*				session_start();
void					session_end(t_session* session);
bool					session_empty(t_session *target);

/*
** Groups
*/
t_group					*group_new();
bool					group_empty(t_group* group);
void					group_insert(t_group* prev, t_group* next, t_group* target);
void					group_remove(t_session** session, t_group** prev, t_group** next);
void					group_push_front(t_session** session, t_group* target);
void					group_push_back(t_session** session, t_group* target);
void					group_pop_front(t_session** session);
void					group_pop_back(t_session** session);

/*
** Process
*/
t_process*				process_new(pid_t pid, int wstatus, char*const* data);
void					process_insert(t_process* prev, t_process* next, t_process* target);
void					process_remove(t_process** prev, t_process** next);
void					process_push_front(t_group** group, t_process* target);
void					process_push_back(t_group** group, t_process* target);
void					process_pop_font(t_group** group);
void					process_pop_back(t_group** group);

/*
** Job Control
*/
void					update_background(t_session* session, t_process **target, bool wait);
bool            		update_session_history(t_session *session, t_process *update);
t_process**				background_find(t_process* target, const char* search_type, t_group* group);
bool					is_active_group(t_group* target);
pid_t					get_process_leader_pid(t_group* nil, t_process* target);
size_t					get_background_index(t_group* nil, t_process* target);
void					force_exit_background(t_session* session);
bool					is_leader(t_session* session, t_process* target);

/*
** Signals catcher
*/
void					zombies_catcher(int signal);
void					suspend_process(int signal);


/*
** Utils
*/
int						parse_flags(int ac, const char* av, const char* pattern);
const char*				is_in_history(t_session* session, t_process* target);
bool					is_not_ambigous(t_session* session, t_process* target);
void					print_index_args(t_session* session, t_process* target);


// old
/*
void            		add_process(t_process* target, t_process* prev, t_process* next);
void            		remove_process(t_process** target);
void            		process_push_front(t_process* process, t_group** group);
void					process_push_back(t_group** group, t_process* target);
t_process*      		new_process(pid_t pid, int wstatus, char*const* data);
void            		group_push_front(t_session* session, t_group* target);
void					group_pop_front(t_session* session);
t_group*        		new_group();
t_session*      		start_session();
void            		delete_process(t_process** target);
void            		delete_group(t_group** target);
bool					is_leader(t_session* session, t_process* target);
void            		end_session(t_session* session);
bool            		update_session_history(t_session* session, t_process* update);
bool					update_background(t_session* session, t_process** process);
bool					update_background_v2(t_session* session, t_process **target);
t_process**				background_find(t_process* target, const char* search_type, t_group* group);
size_t					background_size(t_group* group);
size_t					get_background_index(t_group* group, t_process* target);
pid_t					get_process_leader_pid(t_group* nil, t_process* target);
bool					is_active_group(t_group* target);
t_group**				get_group(t_session* session, t_process* leader);
void					update_groups(t_session* session, t_group** group);
void					force_exit_background(t_session* session);
*/

t_process**				jobspec_parser(t_session* session, int ac, char*const* av, t_process** (*fill)(int ac, char*const* av));
bool					is_string_digit(const char* string);

// old stuff
size_t					suspended_process_nb(t_process* suspended);
bool					is_suspended(int wstatus);
void            		remove_suspended_process(t_process** suspended);
void                	resume_suspended_processes(t_process** suspended);
void					ft_swap(int* a, int* b);
void					update_used_pids(int new, pid_t** used_pids);

#endif
