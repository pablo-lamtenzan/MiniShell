/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:51:14 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 04:36:02 by pablo            ###   ########.fr       */
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
//# include <builtins.h>
# include "bst.h" // to change

# define TOK_NONE       0
# define OP_REDIR_RD    1
# define OP_REDIR_WR    2
# define OP_REDIR_WA    4
# define OP_PIPE        8
# define TOK_CMD        16

# define NONE			TOK_NONE
# define REDIR_GR		OP_REDIR_WR
# define REDIR_LE		OP_REDIR_WA
# define REDIR_DG		OP_REDIR_RD
# define PIPE			OP_PIPE
# define CMD			TOK_CMD
# define FILENAME		32
# define CONST_GR		64 // if set it wont dup2 anymore the > or >>
# define CONST_LE		128 // if set if wont dup2 naymore the <
# define HANDLE_CONST	256

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

int						matrix_height(char*const* matrix);
char**					handle_return_status(char** av, t_term* term);

int						handle_wstatus(int wstatus);
int						execute_child(t_exec* info, t_term* term);
bool					build_execve_args(t_exec** info, t_term* term);
void					destroy_execve_args(t_exec* info);

bool					dup_stdio(int* fds, char* flags);
void					open_pipe_fds(t_exec** info, t_tok_t next_type);
bool					close_pipe_fds(int* fds);
int						redirections_handler(t_exec** info, t_tok_t type, const char* filename);

void					execute_bst(t_bst* root, t_term* term);


void		tests();

#endif