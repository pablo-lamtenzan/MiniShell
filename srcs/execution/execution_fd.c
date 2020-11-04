/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 21:59:46 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h> // to change

bool		dup_stdio(int* fds, char* flags)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		if ((*flags & CONST_GR && i == STDOUT) || (*flags & CONST_LE && i == STDIN))
			continue ;
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
		else if (fds[i] != i) // need this cause we need to only dup2 to the fist file if there are multiple "< or > or >>"
		{
			if (i == 0)
				*flags |= CONST_LE;
			else if (i == 1)
				*flags |= CONST_GR;
		}
	}
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
		if (type > 0 && type & PIPE)
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
	while (++i < 3)
		if (fds[i] == i && close(fds[i] < 0))
			return (false);
	return (true);
}

int			redirections_handler(t_exec** info, t_tok_t type, const char* filename)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	int tmp;
	if (!filename) // don't know if is necesary
		return (0);

	if (type & REDIR_GR)
	{
		tmp = open(filename, O_WRONLY | O_CREAT | O_TRUNC, umask);
		if (!((*info)->handle_dup & CONST_GR)) // testing this
		{
			ft_dprintf(2, "STDOUT IS FIXED!\n");
			(*info)->fds[1] = tmp;
		}
		(*info)->handle_dup |= CONST_GR;
		ft_dprintf(2, "Opened: [%s]. Greather redirecion overrides fds[1]!, Now fds[1] = [ %d ]\n", filename, (*info)->fds[1]);
	}
	else if (type & REDIR_DG)
	{
		tmp = open(filename, O_WRONLY | O_CREAT | O_APPEND, umask);
		if (!((*info)->handle_dup & CONST_GR))
		{
			ft_dprintf(2, "STDOUT IS FIXED!\n");
			(*info)->fds[1] = tmp;
		}
		(*info)->handle_dup |= CONST_GR;
		ft_dprintf(2, "Opened: [%s]. Double greather redirecion overrides fds[1]!, Now fds[1] = [ %d ]\n", filename, (*info)->fds[1]);
	}
	else if (type & REDIR_LE)
	{
		tmp = open(filename, O_RDONLY);
		if (!((*info)->handle_dup & CONST_LE))
		{
			ft_dprintf(2, "STDIN IS FIXED!\n");
			(*info)->fds[0] = tmp;
		}
		(*info)->handle_dup |= CONST_LE;
		ft_dprintf(2, "Opened: [%s]. Leasser redirecion overrides fds[0]!, Now fds[0] = [ %d ]\n", filename, (*info)->fds[0]);
	}
	else
		return (0); // Type is not redir
	return (((*info)->fds[0] | (*info)->fds[1]) >= 0 ? 1 : -1); // -1 fatal error
}