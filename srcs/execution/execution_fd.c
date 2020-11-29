/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 08:03:51 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <expansion.h>
#include <errors.h>

t_exec_status		dup_stdio(int *fds)
{
	int				i;

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

/*
** TODO: Rename READ to WRITE and WRITE to READ
*/

t_exec_status		open_pipe_fds(t_exec **info, t_tok_t type)
{
	bool			update;
	int				pipe_fds[2];

	update = false;
	if ((*info)->fds[FDS_STDOUT] != FDS_STDOUT && (update = true))
	{
		(*info)->fds[FDS_STDOUT] = FDS_STDOUT;
		(*info)->fds[FDS_STDIN] = (*info)->fds[FDS_AUX];
		if (type & PIPE)
			update = false;
	}
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return (BAD_PIPE);
		ft_dprintf(2, "[PIPE] Using %d -> %d pipe for stdout!\n",
			pipe_fds[PIPE_WRITE], pipe_fds[PIPE_READ]);
		(*info)->fds[FDS_STDOUT] = pipe_fds[PIPE_WRITE];
		(*info)->fds[FDS_AUX] = pipe_fds[PIPE_READ];
	}
	return (SUCCESS);
}

t_exec_status		close_pipe_fds(int *fds)
{
	int				i;

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
