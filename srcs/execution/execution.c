/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 02:33:10 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 15:48:57 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

#include <execution.h>
#include <builtins.h>
#include <expansion.h>
#include <job_control.h>
#include <errors.h>

#include <path.h>

// TODO: Handle path concatenation alloc error
static t_exec_status	get_exec(t_exec *info)
{
	t_exec_status	status;

	status = SUCCESS;
	if (!(info->exec = builtin_get(info->av[0])))
	{
		if (!(info->file_path =
			path_get(info->av[0], env_get(info->session->env, "PATH", 4))))
		{
			g_session.st = CMD_NOT_FOUND;
			status = BAD_PATH;
		}
		else if (!(info->ep = (char*const*)env_export(info->session->env)))
			status = RDR_BAD_ALLOC;
		else
			info->exec = &execute_child;
	}
	return (status);
}

static bool				handle_subshell(t_executable exec, const char *name)
{
	if (g_session.flags & PIPED_CMD && (exec == &b_bg 
	|| exec == &b_fg || exec == &b_kill || exec == &b_wait \
	|| exec == &b_disown || exec == &b_jobs))
	{
		ft_dprintf(STDERR_FILENO, "%s: %s: no job control\n", g_session.name, name);
		return (false);
	}
	return (true);
}

static t_exec_status	execute_process(t_exec *info)
{
	t_exec_status		exec_st;

	update_exit_count(info->av[0]);
	if ((exec_st = get_exec(info)) == SUCCESS)
	{
		signal(SIGCHLD, SIG_IGN);
		zombies_list_purge_exited_zombies();
		signal(SIGCHLD, zombies_catcher);
		if (handle_subshell(info->exec, info->av[0]))
			g_session.st = (unsigned char)info->exec(info);
		g_session.groups->active_processes->ret = \
			g_session.groups->active_processes->flags \
			& STOPPED ? -1 : (unsigned char)g_session.st;
	}
	destroy_execve_args(info);
	return (exec_st);
}

static t_exec_status	execute_cmd(t_bst *cmd, t_exec *info)
{
	char				**filename;
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
		info->session = g_session.flags & PIPED_CMD ? session_dup() : &g_session;
		if (!(info->av = tokens_expand((t_tok**)&cmd->a, \
			&info->session->env, &info->ac)))
		{
			g_session.flags & PIPED_CMD ? session_destroy(&info->session) : NULL;
			return (RDR_BAD_ALLOC);
		}
		if (!info->av[0])
		{
			g_session.flags & PIPED_CMD ? session_destroy(&info->session) : NULL;
			return (SUCCESS);
		}
		exec_st = execute_process(info);
		g_session.flags & PIPED_CMD ? session_destroy(&info->session) : NULL;
		if (close_pipe_fds(info->fds) != SUCCESS)
			return (BAD_CLOSE);
	}
	return (exec_st == BAD_PATH ? SUCCESS : exec_st);
}

static t_exec_status	execute_job(t_bst *job, t_exec *info)
{
	t_exec_status		st;

	st = SUCCESS;
	info->handle_dup = NONE;
	if (open_pipe_fds(&info, job->b ? job->type : 0) != SUCCESS)
		return (BAD_PIPE);
	if (!(job->type & (CMD | REDIR_DG | REDIR_GR | REDIR_LE)))
		st = execute_cmd(job->a, info);
	if (st == SUCCESS && job->b && job->type & PIPE)
		st = execute_job(job->b, info);
	else if (st == SUCCESS)
		st = execute_cmd(job, info);
	return (st);
}

t_exec_status			execute_bst(t_bst *root)
{
	t_exec				info;
	t_exec_status		st;
	t_group				*group;

	keep_alive_killed_processes();
	g_session.flags &= ~PIPED_CMD;
	if (!(group = group_new()))
		return (BAD_ALLOC);
	g_session.flags |= OPEN_PRINT;
	group->input = ft_split(g_session.input_line[g_session.input_line_index++],\
		ESPACE);
	group_push_front(group);
	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[FDS_STDOUT]=FDS_STDOUT, .fds[FDS_AUX]=FDS_AUX};
	if (root->type & PIPE && (g_session.flags |= PIPED_CMD))
		st = execute_job(root, &info);
	else
		st = execute_cmd(root, &info);
	return (wait_processes(st));
}
