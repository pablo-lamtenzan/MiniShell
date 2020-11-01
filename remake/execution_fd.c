/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/01 20:19:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h" // to change

bool		dup_stdio(int* fds, char* flags)
{
	int		i;

	i = -1;
	while (++i < 3)
		if (*flags & HANDLE_CONST && (*flags & CONST_GR && i == STDOUT) || (*flags & CONST_LE && i == STDIN))
			continue ;
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
		else if (*flags & HANDLE_CONST && fds[i] != i) // need this cause we need to only dup2 to the fist file if there are multiple "< or > or >>"
		{
			if (i == 0)
				*flags |= CONST_LE;
			else if (i == 1)
				*flags |= CONST_GR;
		}
	return (true);
}

void		open_pipe_fds(t_exec** info, t_tok_t next_type)
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
		if (next_type > 0 && next_type & PIPE | REDIR_GR | REDIR_DG)
			update = false;
	}
	// pipe part 1,3,5,...
	if (!update)
	{
		if (pipe(pipe_fds) < 0)
			return ;

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
	while (++i < 3)
		if (fds[i] == i && close(fds[i] < 0))
			return (false);
	return (true);
}

int			redirections_handler(t_exec** info, t_tok_t type, void* filename)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	if (type & REDIR_GR)
		(*info)->fds[1] = open((char*)filename, O_WRONLY | O_CREAT | O_TRUNC, umask);
	else if (type & REDIR_DG)
		(*info)->fds[1] = open((char*)filename, O_WRONLY | O_CREAT | O_APPEND, umask);
	else if (type & REDIR_LE)
		(*info)->fds[0] = open((char*)filename, O_RDONLY);
	else
		return (0); // Type is not redir
	return ((*info)->fds[0] | (*info)->fds[1] >= 0 ? 1 : -1); // -1 fatal error
}