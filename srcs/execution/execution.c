/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:52:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 22:52:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <builtins.h>
#include <expansion.h>
#include <job_control.h>
#include <errors.h>

static t_exec_status	get_exec(t_exec* info, t_term* term)
{
	const char	*names[] = {"echo", "cd", "pwd", "export", "unset", "env", \
			"exit", "fg", "jobs", "kill", "bg", "disown", "wait"};
	const int	lengths[] = {4, 3, 4, 7, 5, 4, 5, 2, 4, 4, 2, 6, 4};
	const t_executable 	builtins[] = {&ft_echo, &ft_cd, &ft_pwd, &ft_export, \
			&ft_unset, &ft_env, &ft_exit, &ft_fg, &ft_jobs, &ft_kill, &ft_bg, &ft_disown, &ft_wait};
	size_t			i;

	int status = SUCCESS;
	i = 0;
	while (i < sizeof(names) / sizeof(*names) && ft_strncmp(info->av[0], names[i], lengths[i]))
		i++;
	if (i < sizeof(names) / sizeof(*names))
		info->exec = builtins[i];
	else if ((status = build_execve_args(info, term)) == SUCCESS)
		info->exec = &execute_child;
	return (status);
}

static t_exec_status	execute_cmd(t_bst* cmd, t_exec* info, t_term* term)
{
	char**				filename;
	t_redir_status		redir_st;
	t_exec_status		exec_st;

	if ((redir_st = redirections_handler(&info, cmd, term, &filename)) != CONTINUE)
		return (print_redirection_error(redir_st, filename, term));
	if (!(cmd->type & CMD) || (cmd->type & PIPE \
			&& !(((t_bst*)cmd->a)->type & CMD)))
    	exec_st = execute_cmd(cmd->a, info, term);
	else
	{
		if (!(info->av = tokens_expand((t_tok**)&cmd->a, &term->env, &info->ac)))
			return (RDR_BAD_ALLOC);
		if (!info->av[0])
			return (SUCCESS);
		;
		update_exit_count(info->av[0]);
		if ((exec_st = get_exec(info, term)) == SUCCESS)
		{
			g_session->st = (unsigned char)info->exec(info, term);
			g_session->groups->active_processes->ret = g_session->groups->active_processes->flags & STOPPED ? -1 : (unsigned char)g_session->st;
		}
		else
			destroy_execve_args(info);
		if (PRINT_DEBUG)
			ft_dprintf(2, "[EXEC CMD][CURR PROCESS FLAGS: %d][\'%p\']\n", g_session->groups->active_processes->flags, g_session->groups->active_processes);
		if (close_pipe_fds(info->fds) != SUCCESS)
			return (BAD_CLOSE);// return error code (could not BAD_CLOSE to define later)
	}
	return (exec_st == BAD_PATH ? SUCCESS : exec_st);
}

static t_exec_status	execute_job(t_bst* job, t_exec* info, t_term* term)
{
	t_exec_status		st;

	st = SUCCESS; // TODO: init
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
	t_group*			group;


	// TO DO: This shoulb be applied to all the group no only the leaders
	// Used for keep the killed process 1 bst exec more than it shoulb be (used for printing the end status)
	// If i forget: next is for sleep 22 + stop + kill % + sleep 22 + stop + jobs + jobs
	// If i forget: curr is for sleep 22 + stop + kill % + jobs + jobs
	bool	stop = false;
	if (!session_empty() && !group_empty(g_session->groups))
	{
		// Not adding a new background process
		if (g_session->groups->active_processes->flags & SIGNALED && !(g_session->groups->active_processes->flags & KILLED)) // change buy if group siganled and if not group killed
		{
			g_session->groups->active_processes->flags &= ~SIGNALED;
			ft_dprintf(2, "REMOVE SIGANLED FROM CURR \'%p\'\n", g_session->groups->active_processes);
			stop = true;
			group_pop_front();
		}
		// Adding a new background process
		if (g_session->groups->next && g_session->groups->next != g_session->nil)
		{
			if (g_session->groups->next->active_processes->flags & SIGNALED && !(g_session->groups->next->active_processes->flags & KILLED)) // change buy if group siganled and if not group killed
			{
				g_session->groups->next->active_processes->flags &= ~SIGNALED;
				ft_dprintf(2, "REMOVE SIGANLED FROM NEXT \'%p\'\n", g_session->groups->next->active_processes);
				//group_remove_v2(&g_session->groups->next);
				
				// TO DO: I ve alreaddy lost enought time (group address in kill != tahn freed addr but process still the same)
					// I free session nil
					// wtf wtf wtf how ???????
					
				/*
				t_group**	next;
				t_group**	prev;
				next = &g_session->groups->next->next;
				prev = &g_session->groups;
				
				(*next)->prev = *prev;
				(*prev)->next = *next;
				g_session->nil->next = *prev;
				ft_dprintf(2, "prev: %p, next: %p, freed: %p, nil->next: %p\n", *prev, *next, g_session->groups->next, g_session->nil->next);
				free(g_session->groups->next);
				stop = true;
				*/
				
			}
		}
		// Remove killed in all the cases
		if (!stop && g_session->groups->active_processes->flags & KILLED) // change buy if not group killed
		{
			g_session->groups->active_processes->flags &= ~KILLED;
			ft_dprintf(2, "REMOVE KILLED FROM \'%p\'\n", g_session->groups->active_processes);
		}
		
	} 

	
	//remove_exited_zombies();
	if (!(group = group_new()))
		return (BAD_ALLOC);
	g_session->open_print = true;
	group->input = ft_split(g_session->input_line[g_session->input_line_index++], ' ');
/*	if (PRINT_DEBUG) {
	if (g_session->groups && g_session->groups->active_processes)
	{
		ft_dprintf(2, "[XXXXXXXXXXXXXXXXX : %p]\n", g_session->groups->active_processes);
		if (g_session->groups->next && g_session->groups->next->active_processes)
			ft_dprintf(2, "[EXEC BST][PREVIOUS GROUP LEADER FLAGS ARE: %d][\'%p\']\n", g_session->groups->next->active_processes->flags, g_session->groups->next->active_processes);
	}
	}*/
	group_push_front(group);
	//rm_exited_from_history();
	
	ft_bzero(&info, sizeof(t_exec));
	info = (t_exec){.fds[FDS_STDOUT]=FDS_STDOUT, .fds[FDS_AUX]=FDS_AUX};
	if (root->type & PIPE)
		st = execute_job(root, &info, term);
	else
		st = execute_cmd(root, &info, term);
	return (wait_processes_v2(term, st));
}
