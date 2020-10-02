/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 17:24:51 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/02 17:28:06 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <sys/stat.h>
#include <sys/wait.h>

static bool			select_fd(t_pipe2 **p)
{
	if ((*p)->in[0] \
			&& (dup2((*p)->in[0], STDIN_FILENO) < 0 \
			|| close((*p)->in[0]) < 0))
		return (false);
	if ((*p)->fd[1] != STDOUT_FILENO \
			&& (dup2((*p)->fd[1], STDOUT_FILENO) < 0 \
			|| close((*p)->fd[1]) < 0))
		return (false);
	return (true);
}

static bool			execute_child_process(t_pipe2 *p, char **av, uint32_t ac, t_data *data)
{
	char			*execution_path;
	char			**envp;

	if (!(data->pid = fork()))
	{
		if (!(select_fd(&p)))
			return (false);
		if (!is_builting(ac, av, data))
		{
			if (!(get_path_and_envp(&execution_path, &envp, av, data)))
				return (false);
			data->return_status = execve(execution_path, av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			exit(0); // to test
			return (false);
		}
	}
	else if (data->pid < 0)
		return (!(data->return_status = 127));
	while (waitpid(data->pid, NULL, 0) < 0)
		;
	data->pid = 0;
	free_five_ptrs(&execution_path, NULL, &envp, NULL, NULL);
	return (true);
}

static bool			execute_cmd_or_redirection(t_bst *curr, t_data *data)
{
	if (curr->next && (curr->next->operator & REDIRECTION_GREATHER \
			|| curr->next->operator & REDIRECTION_DGREATHER \
			|| curr->next->operator & REDIRECTION_LESSER))
	{
		curr->av[0] = curr->av[1];
		curr->av[1] = curr->next->av[1];
		execute_redirections_cmd(curr, data);
	}
	else
	{
		curr->av[0] = curr->av[1];
		execute_simple_cmd(curr, data);
	}
	return (true);
}

bool				execute_pipe_cmd(t_bst *curr, t_data *data)
{
	t_pipe2			p;
	bool			once;

	p.in[0] = 0;
	once = true;

	while (curr)
	{
		pipe(p.fd);
		if (!execute_child_process(&p, curr->av[once ? 0 : 1], curr->ac[once ? 0 : 1], data) \
				|| close (p.fd[1]) < 0)
			return (false);
		p.in[0] = p.fd[0];
		if (!once)
			curr = curr->next;
		if (!curr->next || (curr->next && !(curr->next->operator & PIPE)))
			break ;
		once = false;
	}
	if (p.in[0] != 0 && dup2(p.in[0], STDIN_FILENO) < 0)
		return (false);
	/* Seems work for:
			cmd | cmd,
			cmd | cmd | cmd,
			cmd | cmd | cmd | cmd,
			cmd | cmd > file and
			cmd | cmd | cmd > file

		The previous while will loop until the last cmd, this last cmd will
				be executed or redirected bellow
	*/	
	return (execute_cmd_or_redirection(curr, data));
}