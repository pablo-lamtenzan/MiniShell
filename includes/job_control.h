/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 11:26:43 by pablo            ###   ########.fr       */
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
# include <signal.h>

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
# define OPEN_PRINT		1
# define RESTRICT_CATCH	2

# define ESPACE			' '

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

typedef struct			s_group
{
	t_process			*active_processes;
	t_process			*nil;
	struct s_group		*next;
	struct s_group		*prev;
	char				**input;
}						t_group;

typedef struct			s_history
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

t_session				g_session;

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
void					zombies_catcher(int signal);
void					signal_handler();

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
void					group_insert(t_group *prev, t_group *next,
						t_group *target);
void					group_remove(t_group **target);
void					group_push_front(t_group *target);
void					group_push_back(t_group *target);
void					group_pop_front();
void					group_pop_back();
bool					group_condition(t_group *target,
						bool (*condition)(t_process*));
t_group					*group_get(t_process *target);
void					group_return_handler();

/*
** Conditions
*/
bool					is_active(t_process *target);
bool					is_exited(t_process *target);
bool					is_removable(t_process *target);
bool					is_coredump(t_process *target);
bool					is_signaled(t_process *target);

/*
** Process
*/
t_process				*process_new(pid_t pid, int wstatus, char*const *data);
void					process_insert(t_process *prev, t_process *next,
						t_process *target);
void					process_remove(t_process **target);
void					process_push_front(t_group **group, t_process *target);
void					process_push_back(t_group **group, t_process *target);
void					process_pop_font(t_group **group);
void					process_pop_back(t_group **group);
pid_t					process_get_leader_pid(t_group *nil, t_process *target);
bool					is_leader(t_process *target);

/*
** Background
*/
void					background_update(t_process **target);
size_t					background_index_get(t_group *nil, t_process *target);
void					background_force_exit();
bool					is_background_active();
t_process				**background_find(t_process *target,
						const char *search_type, t_group	*group);

/*
** Zombies list
*/
bool					zombies_list_update(t_group	*update);
void					zombies_list_remove_node(t_group *target);
void					zombies_list_purge_exited_groups();
void					zombies_list_purge_exited_zombies();

/*
** Dead zombies list
*/
t_deadzombie			*deadzombie_new(t_process *target);
void					deadzombie_push_back(t_deadzombie *target);
void					deadzombies_print();
void					deadzombie_remove_node(t_process *target);

/*
** Exit helper
*/
void					handle_exit_with_active_background(int exit_status);
void					update_exit_count(const char *name);

/*
** Jobspec parser
*/
t_process				**jobspec_parser(int ac, char*const *av,
						bool (*fill)(int ac, char*const *av));
t_process				**get_process_by_name(t_group *groups, const char *av);
bool					is_not_ambigous(t_process *target);
bool					is_not_ambigous_v2(const char *niddle);
bool					is_history_process(const char *string);
size_t					get_search_mode(const char *av);
size_t					get_history_index(const char *key);
bool					is_jobspec(const char *string);

/*
** Static destructors
*/
void					delete_groups();
void					delete_processes(t_group **group);
void					delete_zombies();
void					delete_deadzombies();
void					delete_hist();
void					delete_input_line();
void					delete_group_input(t_group **group);

/*
** Jobs control builtins utils
*/
void					keep_alive_killed_processes();
void					print_process(int fd, t_process *target, int flags);
bool					is_not_running(t_process *target);
bool					is_not_stopped(t_process *target);
void					print_group(int fd, t_process *leader, int flags,
						t_group *itself);
int						wait_process(t_process **target, int flags);
int						wait_group(t_process *leader, int flags,
						t_group *itself);
int						wait_all_groups(int flags);
const char				*get_signal(const char *key, int *res);
void					kill_core(int signal);
int						handle_current(t_process ***target,
						const char *jobspec);
void					kill_group(t_process *leader, int signal,
						t_group *itself);
void					print_all_signals();
void					disown_process(t_process **target, int flags);
int						disowm_delete();
void					disown_core(int flags);
void					disown_all_groups(int flags);
void					disown_group(t_process *leader, int flags,
						t_group *itself);

/*
** Utils
*/
bool					is_string_digit(const char *string);
int						matrix_height(char **matrix);
bool					ignore_pid(int ac, char*const *av);
int						parse_flags(int ac, char*const *av, const char *pattern,
						int *nb_flags);
const char				*is_in_history(t_process *target);
char					**split_separators(char *input,
						const char **separators);
int						for_each_in_group(t_process *leader,
						int (*core)(), bool(*delete)());

static const char		*g_signals[31] = {
	"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT",
	"SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
	"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
	"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
	"SIGXFSZ", "SIGVALRM", "SIGPROF", "SIGWINCH", "SIGIO", "SIGPWR",
	"SIGSYS"
};

static const int		g_values[31] = {
	SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE,
	SIGKILL, SIGUSR1, SIGSEGV, SIGUSR2, SIGPIPE, SIGALRM, SIGTERM,
	SIGSTKFLT, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU,
	SIGURG, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH,
	SIGIO, SIGPWR, SIGSYS
};

static const char		*g_cvalues[31] = { "1", "2", "3", "4", "5", "6", "7",
	"8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
	"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"
};

#endif
