/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/13 05:05:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

#include <stdio.h>

bool		dup_stdio(int* fds)
{
	int		i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i && ft_dprintf(2, "[DUP_STDIO][dup fd:[%d]]\n", i) && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
	return (true);
}

void		open_pipe_fds(t_exec** info, t_tok_t type)
{
	bool	update;
	int		pipe_fds[2];

	update = false;

	// pipe part 2,4,6,... 
	if ((*info)->fds[STDOUT] != STDOUT && (update = true))
	{
		(*info)->fds[STDOUT] = STDOUT;

		// Redirect stdin to pipe read
		(*info)->fds[STDIN] = (*info)->fds[AUX];

		// If inter pipe must redirect stdout to pipe write
		if (type & PIPE)
			update = false;
	}
	// pipe part 1,3,5,...
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return ;

		ft_dprintf(2, "[pipe fds] (in lldb pipe[0] (read pipe) is equal to 0!) values:0:[%d],1:[%d]\n", pipe_fds[0], pipe_fds[1]);

		// Redirect stdout to pipe read
		(*info)->fds[STDOUT] = pipe_fds[READ];

		// Save pipe write
		(*info)->fds[AUX] = pipe_fds[WRITE];
	}
}

bool		close_pipe_fds(int* fds)
{
	int	i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i && ft_dprintf(2,"[Closed: fd[%d]]\n", i) && close(fds[i]) < 0)
			return (false);
	return (true);
}

int			redirections_handler(t_exec** info, t_tok_t type, const char* filename)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	int tmp;
	if (!filename)
		return (0);

	if (type & REDIR_GR)
	{
		tmp = open(filename, O_WRONLY | O_CREAT | O_TRUNC, umask);
		if (!((*info)->handle_dup & CONST_GR))
			(*info)->fds[1] = tmp;
		(*info)->handle_dup |= CONST_GR;
	}
	else if (type & REDIR_DG)
	{
		tmp = open(filename, O_WRONLY | O_CREAT | O_APPEND, umask);
		if (!((*info)->handle_dup & CONST_GR))
			(*info)->fds[1] = tmp;
		(*info)->handle_dup |= CONST_GR;
	}
	else if (type & REDIR_LE)
	{
		tmp = open(filename, O_RDONLY);
		if (!((*info)->handle_dup & CONST_LE))
			(*info)->fds[0] = tmp;
		(*info)->handle_dup |= CONST_LE;
	}
	else
		return (0);
	return (((*info)->fds[0] | (*info)->fds[1]) >= 0 ? 1 : -1); // -1 fatal error
}