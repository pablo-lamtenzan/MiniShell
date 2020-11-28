/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:51:14 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 01:38:38 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <strings.h>

# include <libft.h>
# include <bst.h>
# include <errors.h>
# include <expansion.h>
# include <job_control.h>

/*
** multiple redirections handler
*/
# define REDIR_OUT		1
# define REDIR_IN		2

/*
** file descriptors manager
*/
# define FDS_STDIN		0
# define FDS_STDOUT		1
# define FDS_AUX		2

# define PIPE_WRITE		1
# define PIPE_READ		0

# define UMASK			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

typedef struct s_exec	t_exec;

typedef int	(*t_executable)(t_exec *args);

typedef struct			s_exec
{
	int					fds[3];
	char				handle_dup;
	t_executable		exec;
	char*				execution_path;
	int					ac;
    char*const*			av;
    char*const*			ep;
}						t_exec;

/*
** Utils
*/
int						matrix_height(char **matrix);

/* to remove */
bool					temporally_expansion(t_tok* args, char*** av);

/*
** Exectution fill
*/
int						execute_child(t_exec* info);
t_exec_status			build_execve_args(t_exec* info);
void					destroy_execve_args(t_exec* info);

/*
** Execution fd
*/
t_exec_status			dup_stdio(int* fds);
t_exec_status			open_pipe_fds(t_exec** info, t_tok_t type);
t_exec_status			close_pipe_fds(int* fds);
t_redir_status			redirections_handler(t_exec** info, t_bst* cmd, char*** filename);

/*
** Redirections
*/
t_exec_status			print_redirection_error(t_redir_status rstatus, char** filename);

/*
** Execution
*/
t_exec_status			execute_bst(t_bst* root);
//t_exec_status        	wait_processes(t_term *term, t_exec_status st);
t_exec_status			wait_processes_v2(t_exec_status st);

/*
** Separators
*/
t_tok*					handle_separators(t_tok** tokens, int* status, int* parentheses_nb);
int						handle_conditionals(int parser_st, int* flags, int parentheses_nb);

#endif
