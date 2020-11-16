/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:51:14 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:44:25 by pablo            ###   ########.fr       */
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
# include <term/term.h>
# include <bst.h>
# include <errors.h>
# include <expansion.h>

/*
** multiple redirections handler
*/
# define CONST_GR		1
# define CONST_LE		2

/*
** file descriptors manager
*/
# define STDIN			0
# define STDOUT			1
# define AUX			2
# define READ			1
# define WRITE			0

# define UMASK			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

typedef struct s_exec	t_exec;

typedef int	(*t_executable)(t_exec *args, t_term *term);

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
bool					temporally_expansion(t_tok* args, char*** av, t_term* term);

/*
** Exectution fill
*/
int						execute_child(t_exec* info, t_term* term);
t_exec_status			build_execve_args(t_exec* info, t_term* term);
void					destroy_execve_args(t_exec* info);

/*
** Execution fd
*/
t_exec_status			dup_stdio(int* fds);
t_exec_status			open_pipe_fds(t_exec** info, t_tok_t type);
t_exec_status			close_pipe_fds(int* fds);
t_redir_status			redirections_handler(t_exec** info, t_bst* cmd, t_term* term, char*** filename);

/*
** Redirections
*/
t_exec_status			print_redirection_error(t_redir_status rstatus, char** filename, t_term* term);

/*
** Execution
*/
t_exec_status			execute_bst(t_bst* root, t_term* term);
t_exec_status        	wait_processes(t_term *term, t_exec_status st);

/*
** Separators
*/
t_tok*					handle_separators(t_tok** tokens, int* status, int* parentheses_nb);
int						handle_conditionals(t_term ** term, int parser_st, int* flags, int parentheses_nb);

#endif
