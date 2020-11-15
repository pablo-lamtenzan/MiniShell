/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:44:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <builtins.h>
#include <expansion.h>
#include <process.h>
#include <errors.h>

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
	else if (build_execve_args(info, term) == SUCCESS)
		info->exec = &execute_child;
}

static t_exec_status	execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
{
	char**				filename;
	t_redir_status		redir_st;

	if ((redir_st = redirections_handler(&info, cmd, term, &filename)) != CONTINUE)
		return (print_redirection_error(redir_st, *filename, term));
	if (!(cmd->type & CMD) || (cmd->type & PIPE \
			&& !(((t_bst*)cmd->a)->type & CMD)))
    	execute_cmd(cmd->a, info, term);
	else
	{
		if (!(info->av = token_expand(cmd->a, &term->env, &info->ac)));
			return (BAD_ALLOC);
		if (!info->av[0])
			return (SUCCESS);
		get_exec(info, term);
		if (info->exec)
			term->st = info->exec(info, term);
		else
			destroy_execve_args(info);
		info->exec = NULL;
		if (close_pipe_fds(info->fds) != SUCCESS)
			return (BAD_CLOSE);// return error code (could not BAD_CLOSE to define later)
	}
	return (SUCCESS);
}

static t_exec_status	execute_job(t_bst* job, t_exec* info, t_term* term)
{
	t_exec_status		st;

	info->handle_dup = NONE;
	if (open_pipe_fds(&info, job->b ? job->type : 0) != SUCCESS)
		return (BAD_PIPE);
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)))
    	st = execute_cmd(job->a, info, term);
    if (st == SUCCESS && job->b && job->type & PIPE)
        st = execute_job(job->b, info, term);
	else if (st == SUCCESS) // can i put this else in the second "if" as a ternary ?
		st = execute_cmd(job, info, term);
	return (st);
}

t_exec_status			execute_bst(t_bst* root, t_term* term)
{
	t_exec				info;
	t_exec_status		st;

	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[STDOUT]=STDOUT, .fds[AUX]=AUX};
	if (root->type & PIPE)
		st = execute_job(root, &info, term);
	else
		st = execute_cmd(root, &info, term);
	return (wait_processes(term, st));
}
