/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/01 20:21:36 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h" // to change

static void		get_exec(t_exec** info, t_term* term)
{
	const char	*names[] = {"echo", "cd", "pwd", "export", "unset", "env", \
			"exit"};
	const int	lengths[] = {4, 3, 4, 7, 5, 4, 5};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, \
			&ft_unset, &ft_env, &ft_exit};
	int			i;

	i = 0;
	while (i < 7 && ft_strncmp((*info)->av[0], names[i], lengths[i]))
		i++;
	if (i < 7)
		(*info)->exec = builtins[i];
	else if (build_execve_args(&info, term))
		(*info)->exec = &execute_child;
}

static void		execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
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

// this function is called on a pipe and iterates pipe to pipe until the (pipe/redir) end operator in root's b branch
static void		execute_job(t_bst* job, t_exec* info, t_term* term)
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
	else if (job->type & (REDIR_GR | REDIR_LE | REDIR_DG | CMD) \
			&& (info->handle_dup |= HANDLE_CONST))
		execute_cmd(job, info, term);
}

void			execute_bst(t_bst* root, t_term* term)
{
	t_exec	info;

	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[STDOUT]=STDOUT, .fds[AUX]=AUX};
	if (root->type & PIPE)
		execute_job(root, &info, term);
	else
		execute_cmd(root, &info, term);
}