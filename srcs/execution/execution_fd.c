/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:10:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 05:02:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <expansion.h>

bool		dup_stdio(int* fds)
{
	int		i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
	return (true);
}

bool		open_pipe_fds(t_exec** info, t_tok_t type)
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
			return (false);
		(*info)->fds[STDOUT] = pipe_fds[READ];
		(*info)->fds[AUX] = pipe_fds[WRITE];
	}
	return (true);
}

bool		close_pipe_fds(int* fds)
{
	int		i;

	i = -1;
	while (++i < 2)
		if (fds[i] != i && close(fds[i]) < 0)
			return (false);
	return (true);
}

// to norme
int			redirections_handler(t_exec** info, t_bst* cmd, t_term* term, char*** filename)
{
	static const int	umask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int		tmp;

	if (!cmd->b)
		return (0);
	*filename = (char**)token_expand(cmd->b, &term->env, &(*info)->ac);
	if (!*filename)
		return (-3);
	if (matrix_height(*filename) > 1)
	{
		(*filename)[0] = ((t_tok*)cmd->b)->data;
		return (-2);
	}
	if (cmd->type & REDIR_GR)
	{
		tmp = open((*filename)[0], O_WRONLY | O_CREAT | O_TRUNC, umask);
		if (!((*info)->handle_dup & CONST_GR))
			(*info)->fds[1] = tmp;
		(*info)->handle_dup |= CONST_GR;
	}
	else if (cmd->type & REDIR_DG)
	{
		tmp = open((*filename)[0], O_WRONLY | O_CREAT | O_APPEND, umask);
		if (!((*info)->handle_dup & CONST_GR))
			(*info)->fds[1] = tmp;
		(*info)->handle_dup |= CONST_GR;
	}
	else if (cmd->type & REDIR_LE)
	{
		tmp = open((*filename)[0], O_RDONLY);
		if (!((*info)->handle_dup & CONST_LE))
			(*info)->fds[0] = tmp;
		(*info)->handle_dup |= CONST_LE;
	}
	else
		return (0);
	return (((*info)->fds[0] | (*info)->fds[1]) >= 0 ? 1 : -1);
}