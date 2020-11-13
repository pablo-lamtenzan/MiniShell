/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 04:57:51 by pablo            ###   ########.fr       */
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
	if (redirections_handler(&info, cmd->type, (const char*)cmd->b) < 0)
		return ;
	if (!(cmd->type & CMD) || (cmd->type & PIPE \
			&& !(((t_bst*)cmd->a)->type & CMD)))
    	execute_cmd(cmd->a, info, term);
	else
	{
		temporally_expansion(cmd->a, (char***)&info->av, term);
		info->ac = matrix_height(info->av);
		get_exec(&info, term);
		if (info->exec)
			term->st = info->exec(info, term);
		else
			destroy_execve_args(info);
		info->exec = NULL;
		if (!close_pipe_fds(info->fds))
			return ;
	}
}

static void		execute_job(t_bst* job, t_exec* info, t_term* term)
{
	info->handle_dup = NONE;
	open_pipe_fds(&info, job->b ? job->type : 0);
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)))
    	execute_cmd(job->a, info, term);
    if (job->b && job->type & PIPE)
        execute_job(job->b, info, term);
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