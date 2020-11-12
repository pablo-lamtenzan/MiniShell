/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 00:46:03 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <execution.h>
# include <builtins.h>

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
	else if (build_execve_args(info, term))
		(*info)->exec = &execute_child;
}

static void		execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
{
	// ovewrite info fds and returns true if node is a redirection
	if (redirections_handler(&info, cmd->type, (const char*)cmd->b) < 0)
		return ;
	ft_dprintf(2, "Redirections override! fds = {%d, %d, %d}\n", info->fds[0], info->fds[1], info->fds[2]);

	// goes further on a until find the cmd
	if (!(cmd->type & CMD) || (cmd->type & PIPE && !(((t_bst*)cmd->a)->type & CMD)))
    	execute_cmd(cmd->a, info, term);

	// executes the cmd in the given executions fds
	else
	{
		// init 
		temporally_expansion(cmd->a, (char***)&info->av, term);
		info->ac = matrix_height(info->av);

		// get fct pointer to execution
		get_exec(&info, term);

		// execute
		if (info->exec)
			term->st = info->exec(info, term);
		else
			destroy_execve_args(info);
		info->exec = NULL;
		// close the updated fds in execute_job
		if (!close_pipe_fds(info->fds))
			return ;
	}
}

// this function is called on a pipe and iterates pipe to pipe until the (pipe/redir/cmd) end operator in root's b branch
static void		execute_job(t_bst* job, t_exec* info, t_term* term)
{
	info->handle_dup = NONE;
    // update the executions fds, if last b node after pipe(s) is redir must open the fd and dup here
	open_pipe_fds(&info, job->b ? job->type : 0);
	ft_dprintf(2, "New pipes executed! fds = {%d, %d, %d}\n", info->fds[0], info->fds[1], info->fds[2]);

    // execution of left branch using the updated fds
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)) /* || (job->type & PIPE && job->type & CMD)*/)
    	execute_cmd(job->a, info, term);

    // recursion loop
    if (job->b && job->type & PIPE)
        execute_job(job->b, info, term);

	// exit condition
	else
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