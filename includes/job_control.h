/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 03:51:45 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>

# include <errors.h>



#define	BACKGROUND		1
#define STOPPED			2
#define EXITED			4
#define SIGNALED		8
#define KILLED			16	
#define RESTRICT_OP		32
#define NO_HANGUP		64
#define NO_DELETE		128

typedef struct 			s_process
{
	pid_t				pid;
    int					wstatus;
	unsigned char		flags;
	char*const			*data;
	struct s_process	*next;
	struct s_process	*prev;
	int					ret;
}						t_process;

typedef struct			s_group
{
	t_process			*active_processes;
	t_process			*nil;
	struct s_group		*next;
	struct s_group		*prev;
	char**				input;
}						t_group;

typedef struct 			s_history
{
	t_group				*group;
	struct s_history	*next;
}						t_history;

typedef struct			s_background
{
	t_group				**background_group;
	struct s_background	*next;
}						t_background;

typedef struct			s_deadzombie
{
	t_process			**deadzombie;
	struct s_deadzombie	*next;
}						t_deadzombie;

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
	bool				open_print;

}						t_session;

t_session				*g_session;

t_exec_status			wait_processes(t_exec_status st);

/*
** Session
*/
t_session				*session_start();
void					session_end();
bool					session_empty();

/*
** History
*/
bool					history_session_update(t_group *update);
void					history_pop_front();
void					history_session_remove_node(t_group *target);
void					history_session_purge_exited();

/*
** Groups
*/
t_group					*group_new();
bool					group_empty(t_group *group);
void					group_insert(t_group *prev, t_group *next, t_group* target);
//void					group_remove(t_group** prev, t_group** next);
void					group_remove_v2(t_group **target);
void					group_push_front(t_group *target);
void					group_push_back(t_group *target);
void					group_pop_front();
void					group_pop_back();
bool					group_condition(t_group *target, bool (*condition)(t_process*));
t_group					*group_get(t_process *target);
void					group_return_handler();

/*
** Conditions
*/
bool					is_active(t_process *target);
bool					is_exited(t_process *target);
bool					is_removable(t_process *target);
bool					is_coredump(t_process *target);

/*
** Process
*/
t_process*				process_new(pid_t pid, int wstatus, char*const *data);
void					process_insert(t_process *prev, t_process *next, t_process *target);
void					process_remove(t_process **prev, t_process **next);
void					process_push_front(t_group **group, t_process *target);
void					process_push_back(t_group **group, t_process *target);
void					process_pop_font(t_group **group);
void					process_pop_back(t_group **group);
pid_t					process_get_leader_pid(t_group* nil, t_process *target);
bool					is_leader(t_process *target);

/*
** Background
*/
void					background_update(t_process **target);
size_t					background_index_get(t_group *nil, t_process *target);
void					background_force_exit();
bool					is_background_active();
t_process				**background_find(t_process *target, const char *search_type,
						t_group	*group);

/*
** Zombies list
*/
bool					zombies_list_update(t_group	 **update);
void					zombies_list_node_remove(t_group *target);
void					zombies_list_purge_exited();

/*
** Dead zombies list
*/
t_deadzombie			*deadzombie_new(t_process** target);
void					deadzombie_push_back(t_deadzombie* target);
void					deadzombies_print();

/*
** Exit helper
*/
void					handle_exit_with_active_background(int exit_status);
void					update_exit_count(const char* name);

/*
** Jobspec parser
*/
t_process**				jobspec_parser(int ac, char*const *av, bool (*fill)(int ac, char*const *av));
bool					is_not_ambigous(t_process *target);
bool					is_not_ambigous_v2(const char *niddle);
size_t					get_search_mode(const char *av);
size_t					get_history_index(const char *key);
bool					is_jobspec(const char *string);

/*
** Static destructors
*/
void					delete_groups();
void					delete_processes(t_group *group);
void					delete_zombies();
void					delete_hist();

/*
** Utils
*/
bool					is_string_digit(const char *string);
int						matrix_height(char **matrix);

/* ------------------------------OLD ---------------------------------*/
#define PRINT_DEBUG		0

#define PROCESSES_MAX   4096
#define MANAGE          0

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
void					get_group_return();
t_group*				get_group(t_process* target);
void					rm_exited_from_history();
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


void					delete_groups();
void					delete_processes(t_group* group);
void					delete_zombies();
void					delete_hist();
void					delete_input_line();


/*
** Utils
*/
int						parse_flags(int ac, char*const* av, const char* pattern, int *nb_flags);
const char*				is_in_history(t_process* target);
bool					is_not_ambigous(t_process* target);
bool					is_not_ambigous_v2(const char* niddle);
void					print_index_args(t_process* target);
int						matrix_height(char **matrix);
bool					protect_process(t_group* target);
bool					ignore_pid(int ac, char*const* av);
char**					split_separators(char* input, char** separators);

t_deadzombie*			endzombie_new(t_process** target);
void					endzombie_push_back(t_deadzombie* target);
void					delete_endzombies();
void					print_endzombies();

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

t_process**				jobspec_parser(int ac, char*const* av, bool (*fill)(int ac, char*const* av));
bool					is_string_digit(const char* string);















// old stuff
size_t					suspended_process_nb(t_process* suspended);
bool					is_suspended(int wstatus);
void            		remove_suspended_process(t_process** suspended);
void                	resume_suspended_processes(t_process** suspended);
void					ft_swap(int* a, int* b);
void					update_used_pids(int new, pid_t** used_pids);

#endif
