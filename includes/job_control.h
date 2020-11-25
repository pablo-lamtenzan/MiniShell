/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 00:48:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>

# include <errors.h>

#define PRINT_DEBUG		0

#define PROCESSES_MAX   4096
#define MANAGE          0

#define	BACKGROUND		1
#define STOPPED			2
#define EXITED			4
#define SIGNALED		8		
#define RESTRICT_OP		16
#define NO_HANGUP		32

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
	char**				input;
}						t_group;

typedef struct 			s_history
{
	t_group*			group; // never operate over this just need it for the address
	struct s_history*	next;
}						t_history;

typedef struct			s_background
{
	t_group**			background_group;
	struct s_background*	next;
}						t_background;

typedef struct			s_session
{
	t_group*			groups; // all background processes by group
	t_process			processes[PROCESSES_MAX + 1]; // not used
	t_process			*history;// not used

	t_history*			hist; // change it name later
	t_background*		zombies;
	t_group				*nil;
	unsigned char		exit_count;
	int					st;
	char**				input_line;
	size_t				input_line_index;
	bool				open_print;
}						t_session;

t_session*				g_session;

// new stuff i wish is the last time i redo all

/*
** Session
*/
t_session*				session_start();
void					session_end();
bool					session_empty();

/*
** Groups
*/
t_group					*group_new();
bool					group_empty(t_group* group);
void					group_insert(t_group* prev, t_group* next, t_group* target);
void					group_remove(t_group** prev, t_group** next);
void					group_remove_v2(t_group** target);
void					group_push_front(t_group* target);
void					group_push_back(t_group* target);
void					group_pop_front();
void					group_pop_back();

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
void					update_background(t_process **target, bool wait);
bool            		update_session_history(t_process *update);
void					remove_history_node(t_group* target);
bool					update_session_history_v2(t_group* update);
t_process**				background_find(t_process* target, const char* search_type, t_group* group);
bool					is_active_group(t_group* target);
pid_t					get_process_leader_pid(t_group* nil, t_process* target);
size_t					get_background_index(t_group* nil, t_process* target);
void					force_exit_background();
bool					is_leader(t_process* target);
t_group*				get_group(t_process* target);
void					print_signal(int fd, t_process* target, int mode);
void					handle_exit_with_active_background(int exit_status);
void					update_exit_count(const char* name);

/*
** Signals catcher
*/
void					zombies_catcher(int signal);
void					zombie_catcher_v2(int signal);
bool					update_zombies(t_group** update);
void					remove_exited_zombies();
bool					update_zombies(t_group** update);
void					remove_zombie_node(t_group* target);
void					suspend_process(int signal);


/*
** Utils
*/
int						parse_flags(int ac, char*const* av, const char* pattern, int *nb_flags);
const char*				is_in_history(t_process* target);
bool					is_not_ambigous(t_process* target);
bool					is_not_ambigous_v2(const char* niddle);
void					print_index_args(t_process* target);
int						matrix_height(char **matrix);
char**					split_separators(char* input, char** separators);


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

t_process**				jobspec_parser(int ac, char*const* av, t_process** (*fill)(int ac, char*const* av));
bool					is_string_digit(const char* string);















// old stuff
size_t					suspended_process_nb(t_process* suspended);
bool					is_suspended(int wstatus);
void            		remove_suspended_process(t_process** suspended);
void                	resume_suspended_processes(t_process** suspended);
void					ft_swap(int* a, int* b);
void					update_used_pids(int new, pid_t** used_pids);

#endif
