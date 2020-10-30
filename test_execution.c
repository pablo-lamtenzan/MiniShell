/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 00:52:10 by pablo             #+#    #+#             */
/*   Updated: 2020/10/30 04:05:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

#include <libft.h>

# define NONE       0
# define PIPE       1
# define REDIR_GR   2
# define REDIR_LE   4
# define REDIR_DG   8
# define SEMICOLON  16
# define CMD		32

#define STDIN	0
#define STDOUT	1
#define AUX		2
#define READ	1
#define WRITE	0

#define A_CONST_GR	1 // if set it wont dup2 anymore the > or >>
#define A_CONST_LE	2 // if set if wont dup2 naymore the <
#define B_CONST_GR	4 // if set it wont dup2 anymore the > or >>
#define B_CONST_LE	8 // if set if wont dup2 naymore the <

typedef char    t_tok_t;

typedef struct  s_bst
{
    void*       a;
    void*       b;
    t_tok_t     type;
}               t_bst;

typedef struct  s_exec
{
	int			fds[3];
	char		fidex_fds; // flags used for multiple redirection
	void		*(*exec)();
	int			ac;
    char *const	*av;
    char *const	*ep;
}				t_exec;

// 3 options:
// - root is PIPE: 1 bst (n nodes) -> JOB
// - root is REDIR: 1 bst (n nodes) -> CMD
// - root is NONE: 1 node -> CMD
void execute_cmd(t_bst* cmd, t_exec* info);
void update_info(t_exec** info, t_tok_t next_type);
int redirections_handler(t_exec** info, t_tok_t type, void* filename);

void		execute_bst(t_bst* root)
{
	t_exec	info;

	ft_bzero(&info, sizeof(t_exec));
	info.fds[STDOUT] = STDOUT;
	if (root->type & PIPE)
		execute_job(root, &info);
	else
		execute_cmd(root, &info);
}


// this function is called on a pipe and iterates pipe to pipe until the (pipe/redir) end cmd
// info fd must be 0 and 1 before this fct call
void        execute_job(t_bst* job, t_exec* info)
{
	/* IMPORTANT:
		last b must be a cmd, ALWAYS!
	*/

    // init/update the executions fds
    update_info(&info, job->b ? ((t_bst*)job->b)->type : -1);

    // execution of left branch using the updated fds
    execute_cmd(job->a, info);

    // recursion and end condition (execute_cmd in last b)
    if (job->b && job->type & PIPE)
        execute_job(job->b, info);
    else if (job->b && job->type & (REDIR_GR | REDIR_DG | REDIR_LE))
		; // the redirection must be handled in update info but i can have an undefined nb of redirects
	else
		execute_cmd(job->b, info);
}

// this function is called by execute_job and will execute the left branch of the job
void        execute_cmd(t_bst* cmd, t_exec* info)
{
	// ovewrite info fds and returns true if node is a redirection
	redirections_handler(&info, cmd->type, cmd->b);
	// goes left until find the cmd
	if (!(cmd->type & CMD))
    	execute_cmd(cmd->a, info);
	// executes the cmd in the given exections fds
	else
	{
		info->ac = 0; // to calc somewhere
		info->av = cmd->a;
		info->ep = NULL; // to get somewhere
		info->exec = get_exec(info->av[0]);
		info->exec(info);

		/* IMP: Put where i gonna do the dup2 the fixed fds */
	}
}

void        update_info(t_exec** info, t_tok_t next_type)
{
	bool	update;
	int		pipe_fds[2];

	update = false;
	
    // no more to execute
    if (next_type < 0)
        return ;

	// pipe part 2,4,6... 
	if ((*info)->fds[STDOUT] != STDOUT && (update = true))
	{
		(*info)->fds[STDOUT] = STDOUT;
		// Redirect stdin to pipe read
		(*info)->fds[STDIN] = (*info)->fds[AUX];
		// Inter pipe ? redirect stdout
		if (next_type & PIPE)
			update = false;
		else if (next_type & (REDIR_GR | REDIR_DG | REDIR_LE)) // redir (only can be the last b)
			; // open fd, dup 2 stdout/in to fd, and put the fixed fd
	}
	// pipe part 1,3,5...
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return (false);
		// Redirect stdout to pipe read
		(*info)->fds[STDOUT] = pipe_fds[READ];
		// Save stdin
		(*info)->fds[AUX] = pipe_fds[AUX];
		// have to close this after execution
	}
}

int		redirections_handler(t_exec** info, t_tok_t type, void* filename)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	if (type & REDIR_GR)
		(*info)->fds[1] = open((char*)filename, O_WRONLY | O_CREAT | O_TRUNC, umask);
	else if (type & REDIR_DG)
		(*info)->fds[1] = open((char*)filename, O_WRONLY | O_CREAT | O_APPEND, umask);
	else if (type & REDIR_LE)
		(*info)->fds[0] = open((char*)filename, O_RDONLY);
	else
		return (0); // Type is not redir
	return ((*info)->fds[0] | (*info)->fds[1] >= 0 ? 1 : -1); // -1 fatal error
}