/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 00:12:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

/*
** External
*/
# include <unistd.h>
# include <stdbool.h>
# include <sys/types.h>

/*
** Local
*/
# include <env.h>
# include <errors.h>

/*
** Process flags
*/
# define BACKGROUND		1
# define STOPPED		2
# define EXITED			4
# define SIGNALED		8
# define KILLED			16	
# define RESTRICT_OP	32
# define NO_HANGUP		64
# define NO_DELETE		128

/*
** Session flags
*/
#define OPEN_PRINT		1
#define RESTRICT_CATCH	2

#define ESPACE			' '

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
	t_group				*background_group;
	bool				exited;
	struct s_background	*next;
}						t_background;

typedef struct			s_deadzombie
{
	t_process			*deadzombie;
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
	char				*name;
	t_env				*env;
	char				flags;
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
** Signals interactions
*/
void					zombie_catcher(int signal);

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
void					group_insert(t_group *prev, t_group *next, t_group *target);
//void					group_remove(t_group** prev, t_group** next);
void					group_remove_v2(t_group **target);
void					group_remove(t_group **target);
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
//void					process_remove(t_process** target);
void					remove_process(t_process** target);
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
bool					zombies_list_update(t_group	 *update);
void					zombies_list_remove_node(t_group *target);
void					zombies_list_purge_exited_groups();
void					zombies_list_purge_exited_zombies();

/*
** Dead zombies list
*/
t_deadzombie			*deadzombie_new(t_process* target);
void					deadzombie_push_back(t_deadzombie* target);
void					deadzombies_print();
void					deadzombie_remove_node(t_process *target);

/*
** Exit helper
*/
void					handle_exit_with_active_background(int exit_status);
void					update_exit_count(const char* name);

/*
** Jobspec parser
*/
t_process**				jobspec_parser(int ac, char*const *av, bool (*fill)(int ac, char*const *av));
t_process**				get_process_by_name(t_group* groups, const char* av);
bool					is_not_ambigous(t_process *target);
bool					is_not_ambigous_v2(const char *niddle);
bool					is_history_process(const char* string);
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
bool					ignore_pid(int ac, char*const* av);
int						parse_flags(int ac, char*const *av, const char* pattern, int *nb_flags);
const char				*is_in_history(t_process* target);
char					**split_separators(char *input, const char **separators);

#endif
