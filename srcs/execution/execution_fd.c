/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:44:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <expansion.h>
#include <errors.h>

t_exec_status		dup_stdio(int* fds)
{
	int		i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i)
		{
			if (dup2(fds[i], i) < 0)
				return (BAD_DUP2);
			if (close(fds[i]) < 0)
				return (BAD_CLOSE);
		}
	return (SUCCESS);
}

t_exec_status		open_pipe_fds(t_exec** info, t_tok_t type)
{
	bool	update;
	int		pipe_fds[2];

	update = false;

	if ((*info)->fds[STDOUT] != STDOUT && (update = true))
	{
		(*info)->fds[STDOUT] = STDOUT;
		(*info)->fds[STDIN] = (*info)->fds[AUX];
		if (type & PIPE)
			update = false;
	}
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return (BAD_PIPE);
		(*info)->fds[STDOUT] = pipe_fds[READ];
		(*info)->fds[AUX] = pipe_fds[WRITE];
	}
	return (SUCCESS);
}

t_exec_status		close_pipe_fds(int* fds)
{
	int		i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i && close(fds[i]) < 0)
			return (BAD_CLOSE);
	return (SUCCESS);
}
