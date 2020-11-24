/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/24 11:10:50 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>
#include <errors.h>
#include <errno.h>

static int	ft_fork(t_exec* info, t_term* term) 
{
	(void)term;
	int		child_st;

	child_st = fork();

	// old stuff
	//term->session->processes[term->session->processes[MANAGE].pid].pid = child_st;
	//term->session->processes[term->session->processes[MANAGE].pid].data = info->av;

	// necesarry
	//if (!ft_strncmp((const char*)term->session->processes[term->session->processes[MANAGE].pid].data, "./", 2))
		; // TO DO: trim ./
	
	// New Stuff
	if (child_st > 0) {
	t_process*		process;

	if (!(process = process_new(child_st, 0, info->av)))
		return (-1);
	//ft_dprintf(2, "Before push back: %p\n", term->session->groups);
	process_push_back(&g_session->groups, process);
	//ft_dprintf(2, "[FT FORK][PUSH BACK: groups are now %p]\n", term->session->groups);
	}
	// old stufff
	if (child_st == 0)
	{
		// for the moment overwrite the last
		//if (term->session->processes[MANAGE].pid < PROCESSES_MAX)
		//{
		//	term->session->processes[MANAGE].pid++;
		//}
	}
	return (child_st);
}

int		execute_child(t_exec* info, t_term* term)
{
	int				ret;
	pid_t			pid;
	t_exec_status	st;

	ret = 0;
	if (!(pid = ft_fork(info, term)))
	{
		if ((st = dup_stdio(info->fds)) != SUCCESS)
			return (st);
		ret = execve(info->execution_path, info->av, info->ep);
		ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], ret);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		ft_dprintf(2, "Fork returnd -1!\n");
		return (errno);
	}
	if (PRINT_DEBUG)
		ft_dprintf(2, "[NEW FORK][pid=\'%d\', name=\'%s\']\n", pid, info->av[0]);
	//while (waitpid(term->pid, &wstatus, 0) <= 0)
	//	;
	//return (handle_wstatus(wstatus, info->av));
	//term->session->processes[term->session->processes[MANAGE].pid].wstatus = 0;
	//term->session->groups->active_processes->wstatus = ret;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[EXECUTE CHILD][RET: %d]\n", ret);
	return (ret);
}

t_exec_status		build_execve_args(t_exec* info, t_term* term)
{
	if (!(info->execution_path = path_get(info->av[0], env_get(term->env, "PATH", 4))))
	{
		g_session->st = CMD_NOT_FOUND;
		return (BAD_PATH);
	}
	if (!(info->ep = (char*const*)env_export(term->env)))
	{
		free(info->execution_path);
		return (RDR_BAD_ALLOC);
	}
	return (SUCCESS);
}

void		destroy_execve_args(t_exec *info)
{
	free((char **)info->ep);
	free((void*)info->execution_path);
	info->exec = NULL;
}

/*
int			matrix_height(char **matrix)
{
	char** it;

	it = matrix;
	while (*it)
		it++;
	return (it - matrix);
}
*/