/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:51:14 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 22:05:47 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

/*
** External
*/
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
# include <errno.h>
# include <strings.h>

/*
** Local
*/
# include <bst.h>
# include <job_control.h>
# include <libft.h>
# include <expansion.h>
# include <errors.h>


/*
** Multiple redirections handler
*/
# define REDIR_OUT		1
# define REDIR_IN		2

/*
** File descriptors manager indexes
*/
# define FDS_STDIN		0
# define FDS_STDOUT		1
# define FDS_AUX		2

/*
** Pipes file descriptors indexes
*/
# define PIPE_WRITE		1
# define PIPE_READ		0

# define UMASK			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

typedef struct s_exec	t_exec;

typedef int				(*t_executable)(t_exec *args);

typedef struct			s_exec
{
	int					fds[3];
	char				handle_dup;
	t_executable		exec;
	char				*file_path;
	int					ac;
    char*const			*av;
    char*const			*ep;
	t_session			*session;
}						t_exec;

/*
** Execution
*/
t_exec_status			execute_bst(t_bst *root);

/*
** Execution fd
*/
t_exec_status			dup_stdio(int *fds);
t_exec_status			open_pipe_fds(t_exec **info, t_tok_t type);
t_exec_status			close_pipe_fds(int *fds);
t_redir_status			redirections_handler(t_exec **info, t_bst *cmd, char ***filename);

/*
** Exectution fill
*/
int						execute_child(t_exec *info);
t_exec_status			build_execve_args(t_exec *info);
void					destroy_execve_args(t_exec *info);
bool					handle_subshell(t_executable exec, const char *name);
t_exec_status			get_exec(t_exec *info);

/*
** Redirections
*/
t_exec_status			print_redirection_error(t_redir_status rstatus, char **filename);

/*
** Separators
*/
t_tok					*handle_separators(t_tok **tokens, int *status, int *parentheses_nb);
int						handle_conditionals(int parser_st, int *flags, int parentheses_nb);

#endif
