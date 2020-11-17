/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 10:13:43 by chamada          ###   ########.fr       */
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

// TODO: Rename READ to WRITE and WRITE to READ
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
		ft_dprintf(2, "[PIPE] Using %d -> %d pipe for stdout!\n", pipe_fds[READ], pipe_fds[WRITE]);
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
	{
		if (fds[i] != i)
		{
			if (close(fds[i]) < 0)
				return (BAD_CLOSE);
			ft_dprintf(2, "[PIPE] Closed fds[%d] = %d!\n", i, fds[i]);
		}
	}
	return (SUCCESS);
}
