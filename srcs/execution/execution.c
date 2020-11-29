/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 02:33:10 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 04:46:49 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <builtins.h>
#include <expansion.h>
#include <job_control.h>
#include <errors.h>

static t_exec_status	get_exec(t_exec *info)
{
	size_t				i;
	int 				status;
	const char			*names[] = {"echo", "cd", "pwd", "export", "unset", \
		"env", "exit", "fg", "jobs", "kill", "bg", "disown", "wait"};
	const int			lengths[] = {4, 3, 4, 7, 5, 4, 5, 2, 4, 4, 2, 6, 4};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, \
		&ft_unset, &ft_env, &ft_exit, &ft_fg, &ft_jobs, &ft_kill, &ft_bg, \
		&ft_disown, &ft_wait};

	status = SUCCESS;
	i = 0;
	while (i < sizeof(names) / sizeof(*names) && ft_strncmp(info->av[0], \
			names[i], lengths[i]))
		i++;
	if (i < sizeof(names) / sizeof(*names))
		info->exec = builtins[i];
	else if ((status = build_execve_args(info)) == SUCCESS)
		info->exec = &execute_child;
	return (status);
}

void					execute_process(t_exec *info, t_exec_status exec_st)
{
	update_exit_count(info->av[0]);
	if ((exec_st = get_exec(info)) == SUCCESS)
	{
		g_session.flags |= RESTRICT_CATCH;
		zombies_list_purge_exited_zombies();
		g_session.flags &= ~RESTRICT_CATCH;
		g_session.st = (unsigned char)info->exec(info);
		g_session.groups->active_processes->ret = \
			g_session.groups->active_processes->flags \
			& STOPPED ? -1 : (unsigned char)g_session.st;
	}
	else
		destroy_execve_args(info);
}

static t_exec_status	execute_cmd(t_bst *cmd, t_exec *info)
{
	char**				filename;
	t_redir_status		redir_st;
	t_exec_status		exec_st;

	exec_st = SUCCESS;
	if ((redir_st = redirections_handler(&info, cmd, &filename)) != CONTINUE)
		return (print_redirection_error(redir_st, filename));
	if (!(cmd->type & CMD) || (cmd->type & PIPE \
			&& !(((t_bst*)cmd->a)->type & CMD)))
    	exec_st = execute_cmd(cmd->a, info);
	else
	{
		if (!(info->av = tokens_expand((t_tok**)&cmd->a, &g_session.env, &info->ac)))
			return (RDR_BAD_ALLOC);
		if (!info->av[0])
			return (SUCCESS);
		execute_process(info, exec_st);
		if (close_pipe_fds(info->fds) != SUCCESS)
			return (BAD_CLOSE);
	}
	return (exec_st == BAD_PATH ? SUCCESS : exec_st);
}

static t_exec_status	execute_job(t_bst *job, t_exec *info)
{
	t_exec_status		st;

	st = SUCCESS; // TODO: init
	info->handle_dup = NONE;
	if (open_pipe_fds(&info, job->b ? job->type : 0) != SUCCESS)
		return (BAD_PIPE);
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)))
    	st = execute_cmd(job->a, info);
    if (st == SUCCESS && job->b && job->type & PIPE)
        st = execute_job(job->b, info);
	else if (st == SUCCESS) // can i put this else in the second "if" as a ternary ?
		st = execute_cmd(job, info);
	return (st);
}

t_exec_status			execute_bst(t_bst *root)
{
	t_exec				info;
	t_exec_status		st;
	t_group*			group;

	keep_alive_killed_processes();
	if (!(group = group_new()))
		return (BAD_ALLOC);
	g_session.flags |= OPEN_PRINT;
	group->input = ft_split(g_session.input_line[g_session.input_line_index++], ESPACE);
	group_push_front(group);
	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[FDS_STDOUT]=FDS_STDOUT, .fds[FDS_AUX]=FDS_AUX};
	if (root->type & PIPE)
		st = execute_job(root, &info);
	else
		st = execute_cmd(root, &info);
	return (wait_processes(st));
}
