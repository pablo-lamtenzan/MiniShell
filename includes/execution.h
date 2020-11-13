/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:51:14 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 05:44:50 by pablo            ###   ########.fr       */
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

# include <libft.h>
# include <term/term.h>
# include <bst.h>

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

typedef struct s_exec	t_exec;

typedef int				(*t_executable)(t_exec *args, t_term *term);

typedef struct			s_exec
{
	int					fds[3];
	char				handle_dup;
	t_executable		exec;
	const char*			execution_path;
	int					ac;
    char*const*			av;
    char*const*			ep;
}						t_exec;

/*
** Utils
*/
int						matrix_height(char*const* matrix);

/* to remove */
bool					temporally_expansion(t_tok* args, char*** av, t_term* term);

/*
** Exectution fill
*/
int						handle_wstatus(int wstatus);
int						execute_child(t_exec* info, t_term* term);
bool					build_execve_args(t_exec** info, t_term* term);
void					destroy_execve_args(t_exec* info);

/*
** Execution fd
*/
bool					dup_stdio(int* fds);
bool					open_pipe_fds(t_exec** info, t_tok_t type);
bool					close_pipe_fds(int* fds);
int						redirections_handler(t_exec** info, t_tok_t type, const char* filename);

/*
** Execution
*/
bool					execute_bst(t_bst* root, t_term* term);

/*
** Separators
*/
t_tok*					handle_separators(t_tok** tokens, int* status, int* parentheses_nb);
int						handle_conditionals(t_term ** term, int parser_st, int* flags, int parentheses_nb);

#endif