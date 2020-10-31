/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 00:52:10 by pablo             #+#    #+#             */
/*   Updated: 2020/10/30 23:41:46 by pablo            ###   ########.fr       */
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

#define CONST_GR	1 // if set it wont dup2 anymore the > or >>
#define CONST_LE	2 // if set if wont dup2 naymore the <

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
	char			fidex_fds; // flags used for multiple redirection
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
void open_pipe_fds(t_exec** info, t_tok_t next_type, char* filename);
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
	/* IMPORTANT:
		last b must be a cmd, ALWAYS!
	*/

    // update the executions fds, if last b node after pipe(s) is redir must open the fd and dup here
    open_pipe_fds(&info, job->b ? ((t_bst*)job->b)->type : -1, job->b && ((t_bst*)job->b)->type & (REDIR_GR | REDIR_DG | REDIR_LE) ? ((t_bst*)job->b)->b : NULL);

    // execution of left branch using the updated fds
	if (!(job->type & CMD))
    	execute_cmd(job->a, info, term);

    // recursion loop
    if (job->b && job->type & PIPE)
        execute_job(job->b, info, term);

	// exit conditions
    else if (job->b && job->type & (REDIR_GR | REDIR_DG | REDIR_LE)) // n pipe(s) ending by a redirection (redirection alreaddy executed in prev node)
		return ;
	else if (job->type & CMD) // n pipe(s) ending by a cmd
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

void        open_pipe_fds(t_exec** info, t_tok_t next_type, char* filename)
{
	bool	update;
	int		pipe_fds[2];

	update = false;
	
    // no more fds to open
    if (next_type < 0)
        return ;

	// pipe part 2,4,6,... 
	if ((*info)->fds[STDOUT] != STDOUT && (update = true))
	{
		(*info)->fds[STDOUT] = STDOUT;

		// Redirect stdin to pipe read
		(*info)->fds[STDIN] = (*info)->fds[AUX];

		// If inter pipe must redirect stdout to pipe write
		if (next_type & PIPE)
			update = false;
		
		// If next b is redir must execute the redir one node before
		else if (next_type & (REDIR_GR | REDIR_DG | REDIR_LE) && filename \
				&& (redirections_handler(info, next_type, filename) < 0 \
				|| !dup_stdio((*info)->fds, (*info)->fidex_fds)))
			return ;
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
		if ((*flags & CONST_GR && i == STDOUT) || (*flags & CONST_LE && i == STDIN))
			continue ;
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
		else if (fds[i] != i) // need this cause we need to only dup2 to the fist file if there are multiple "< or > or >>"
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
		if (dup_stdio(info->fds, &info->fidex_fds))
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