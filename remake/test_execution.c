/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 00:52:10 by pablo             #+#    #+#             */
/*   Updated: 2020/11/01 20:00:04 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>

#include <errno.h>

#include <libft.h>
#include <builtins.h>

# define TOK_NONE       0
# define OP_REDIR_RD    1
# define OP_REDIR_WR    2
# define OP_REDIR_WA    4
# define OP_PIPE        8
# define TOK_CMD        16

# define NONE       TOK_NONE
# define REDIR_GR   OP_REDIR_WR
# define REDIR_LE   OP_REDIR_WA
# define REDIR_DG   OP_REDIR_RD
# define PIPE       OP_PIPE
# define CMD		TOK_CMD
# define FILENAME	32
# define CONST_GR	64 // if set it wont dup2 anymore the > or >>
# define CONST_LE	128 // if set if wont dup2 naymore the <
# define HANDLE_CONST 256

#define STDIN	0
#define STDOUT	1
#define AUX		2
#define READ	1
#define WRITE	0


typedef char    t_tok_t;

typedef struct  s_bst
{
    void*       a;
    void*       b;
    t_tok_t     type;
}               t_bst;

t_exec;

typedef int	(*t_executable)(t_exec *args, t_term *term);
typedef struct s_term
{
	t_map			*env;
	char			*name;
	pid_t			pid;
	int				st;
	struct termios	s_ios;
	struct termios	s_ios_bkp;
	t_line			*line;
	t_caps			caps;
	t_cursor		cursor;
	t_clip			clip;
	t_hist			hist;
	int				(*exec)(const char*, struct s_term*);
	
};				t_term;


typedef struct  s_exec
{
	int				fds[3];
	char			handle_dup;
	t_executable	exec;
	const char*		execution_path;
	int				ac;
    char*const*		av;
    char*const*		ep;
}				t_exec;

// 3 cases:
// - root is PIPE: 1 bst (n nodes) -> JOB
// - root is REDIR: 1 bst (n nodes) -> CMD
// - root is CDM: 1 node -> CMD
void execute_cmd(t_bst* cmd, t_exec* info, t_term* term);
void open_pipe_fds(t_exec** info, t_tok_t next_type);
int redirections_handler(t_exec** info, t_tok_t type, void* filename);
void get_exec(t_exec** info, t_term* term);
bool close_pipe_fds(int* fds);
void destroy_execve_args(t_exec* info);

void		execute_bst(t_bst* root, t_term* term)
{
	t_exec	info;

	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[STDOUT]=STDOUT, .fds[AUX]=AUX};
	if (root->type & PIPE)
		execute_job(root, &info, term);
	else
		execute_cmd(root, &info, term);
}

// this function is called on a pipe and iterates pipe to pipe until the (pipe/redir) end operator in root's b branch
void        execute_job(t_bst* job, t_exec* info, t_term* term)
{
    // update the executions fds, if last b node after pipe(s) is redir must open the fd and dup here
	open_pipe_fds(&info, job->b ? ((t_bst*)job->b)->type : -1);

    // execution of left branch using the updated fds
	if (!(job->type & CMD))
    execute_cmd(job->a, info, term);

    // recursion loop
    if (job->b && job->type & PIPE)
        execute_job(job->b, info, term);

	// exit condition
	else if (job->type & (REDIR_GR | REDIR_LE | REDIR_DG | CMD) && (info->handle_dup |= HANDLE_CONST))
		execute_cmd(job, info, term);
}

// this function is called by execute_job and will execute the left branch of the job
void        execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
{
	// ovewrite info fds and returns true if node is a redirection
	if (redirections_handler(&info, cmd->type, cmd->b) < 0)
		return ;

	// goes further on a until find the cmd
	if (!(cmd->type & CMD))
    	execute_cmd(cmd->a, info, term);

	// executes the cmd in the given executions fds
	else
	{
		// init 
		info->ac = 0; // to calc somewhere
		info->av = cmd->a;

		// get fct pointer to execution
		get_exec(&info, term);

		// execute
		if (info->exec)
			info->exec(info, term);
		else
			destroy_execve_args(info);

		// close the updated fds in execute_job
		if (!close_pipe_fds(info->fds))
			return ;
	}
}

void        open_pipe_fds(t_exec** info, t_tok_t next_type)
{
	bool	update;
	int		pipe_fds[2];

	update = false;

	// pipe part 2,4,6,... 
	if ((*info)->fds[STDOUT] != STDOUT && (update = true))
	{
		(*info)->fds[STDOUT] = STDOUT;

		// Redirect stdin to pipe read
		(*info)->fds[STDIN] = (*info)->fds[AUX];

		// If inter pipe must redirect stdout to pipe write
		if (next_type > 0 && next_type & PIPE | REDIR_GR | REDIR_DG)
			update = false;
	}
	// pipe part 1,3,5,...
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return ;

		// Redirect stdout to pipe read
		(*info)->fds[STDOUT] = pipe_fds[READ];

		// Save pipe write
		(*info)->fds[AUX] = pipe_fds[WRITE];
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

/*
static int	handle_wstatus(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	return (wstatus);
}
*/

static int	handle_wstatus(int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		wstatus = WEXITSTATUS(wstatus);
		dprintf(2, "[exec][status] child exited with status '%d'!\n", wstatus);
		return (wstatus);
	}
	if (WIFSIGNALED(wstatus))
	{
		wstatus = WTERMSIG(wstatus);
		dprintf(2, "[exec][status] %s\n", strsignal(wstatus));
		return (wstatus);
	}
	dprintf(2, "[exec][status] cannot retrieve child status!\n");
	return (wstatus);
}

bool	dup_stdio(int* fds, char* flags)
{
	int i;

	i = -1;
	while (++i < 3)
		if (*flags & HANDLE_CONST && (*flags & CONST_GR && i == STDOUT) || (*flags & CONST_LE && i == STDIN))
			continue ;
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
		else if (*flags & HANDLE_CONST && fds[i] != i) // need this cause we need to only dup2 to the fist file if there are multiple "< or > or >>"
		{
			if (i == 0)
				*flags |= CONST_LE;
			else if (i == 1)
				*flags |= CONST_GR;
		}
	return (true);
}

int execute_child(t_exec* info, t_term* term)
{
	int		wstatus;

	if (!(term->pid = fork()))
	{
		if (dup_stdio(info->fds, &info->handle_dup))
		{
			wstatus = execve(info->execution_path, info->av, info->ep);
			ft_dprintf(2, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		}
		exit(EXIT_FAILURE);
	}
	else if (term->pid < 0)
		return (errno);
	while (waitpid(term->pid, &wstatus, 0) <= 0)
		;
	return (handle_wstatus(wstatus));
}

bool	build_execve_args(t_exec** info, t_term* term)
{
	t_map*	path;

	if (!(path = map_get(term->env, "PATH")) \
			|| !path->value \
			|| !((*info)->execution_path = path_get((*info)->av[0], path->value)))
		return (!(term->st = 127));
	if (!((*info)->ep = map_export(term->env)))
	{
		free((*info)->execution_path);
		return (false);
	}
	return (true);
}

void	get_exec(t_exec** info, t_term* term)
{
	const char			*names[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
	const int			lengths[] = {4, 3, 4, 7, 5, 4, 5};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, &ft_unset, &ft_env, &ft_exit};
	int					i;

	i = 0;
	while (i < 7 && ft_strncmp((*info)->av[0], names[i], lengths[i]))
		i++;
	if (i < 7)
		(*info)->exec = builtins[i];
	else if (build_execve_args(&info, term))
		(*info)->exec = &execute_child;
}

bool	close_pipe_fds(int* fds)
{
	int	i;

	i = -1;
	while (++i < 3)
		if (fds[i] == i && close(fds[i] < 0))
			return (false);
	return (true);
}

void	destroy_execve_args(t_exec* info)
{
	char**	aux;
	int		i;

	aux = (char**)info->ep;
	i = 0;
	while (aux && aux[i++])
		free(aux);
	free(aux);
	free(info->execution_path);
}