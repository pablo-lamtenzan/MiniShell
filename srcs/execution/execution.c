/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 04:56:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <builtins.h>
#include <expansion.h>

static void		get_exec(t_exec* info, t_term* term)
{
	const char	*names[] = {"echo", "cd", "pwd", "export", "unset", "env", \
			"exit"};
	const int	lengths[] = {4, 3, 4, 7, 5, 4, 5};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, \
			&ft_unset, &ft_env, &ft_exit};
	int			i;

	i = 0;
	while (i < 7 && ft_strncmp(info->av[0], names[i], lengths[i]))
		i++;
	if (i < 7)
		info->exec = builtins[i];
	else if (build_execve_args(info, term))
		info->exec = &execute_child;
}

static bool		execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
{
	char**		filename;
	int			redir_st;

	if ((redir_st = redirections_handler(&info, cmd, term, &filename)) < 0)
		return (ft_dprintf(2, "minish: %s: No such file or directory\n", filename[0]));
	else if (redir_st < -1)
		return (false);
	if (!(cmd->type & CMD) || (cmd->type & PIPE \
			&& !(((t_bst*)cmd->a)->type & CMD)))
    	execute_cmd(cmd->a, info, term);
	else
	{
		info->av = token_expand(cmd->a, &term->env, &info->ac);
		if (!info->av)
			return (false);
		if (!info->av[0])
			return (true);
		get_exec(info, term);
		if (info->exec)
			term->st = info->exec(info, term);
		else
			destroy_execve_args(info);
		info->exec = NULL;
		if (!close_pipe_fds(info->fds))
			return (false);
	}
	return (true);
}

static bool		execute_job(t_bst* job, t_exec* info, t_term* term)
{
	bool		success;

	success = true;
	info->handle_dup = NONE;
	if (!open_pipe_fds(&info, job->b ? job->type : 0))
		return (false);
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)))
    	success *= execute_cmd(job->a, info, term);
    if (job->b && job->type & PIPE)
        success *= execute_job(job->b, info, term);
	else
		success *= execute_cmd(job, info, term);
	return (success);
}

bool			execute_bst(t_bst* root, t_term* term)
{
	t_exec		info;

	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[STDOUT]=STDOUT, .fds[AUX]=AUX};
	if (root->type & PIPE)
		return (execute_job(root, &info, term));
	return (execute_cmd(root, &info, term));
}
