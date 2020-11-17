/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 08:52:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 17:25:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

t_exec_status			print_redirection_error(t_redir_status rstatus, char** filename, t_term* term)
{
	const char*	error_msg[3] = {
		"minish: %s: No such file or directory\n",
		"minish: %s: ambigous redirect\n",
		"minish: %s: File name too long\n"
	};
	
	if (rstatus == RDR_BAD_ALLOC)
		return (RDR_BAD_ALLOC);
	ft_dprintf(STDERR_FILENO, error_msg[-rstatus - 1], filename);
	term->st = STD_ERROR;
	return (SUCCESS);
}

static t_redir_status	try_catch_filename(char*** filename, char* var_name, int height)
{
    if (height != 1)
	{
		(*filename)[0] = var_name;
		return (AMBIG_REDIRECT);
	}
	if (ft_strlen((*filename)[0]) >= PATH_MAX)
		return (FLNAME_TO_LONG);
    return (CONTINUE);
}

static t_redir_status	try_catch_in(t_exec** info, t_tok_t type, const char* filename)
{
	int					tmp;
	(void)type;

	tmp = open(filename, O_RDONLY);
	if (!((*info)->handle_dup & CONST_LE))
		(*info)->fds[0] = tmp;
	(*info)->handle_dup |= CONST_LE;
	return (tmp >= 0 ? CONTINUE : FILE_NOT_FOUND);
}

static t_redir_status	try_catch_out(t_exec** info, t_tok_t type, const char* filename)
{
	int	tmp;

	tmp = -1; // TODO: Init
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
	return (tmp >= 0 ? CONTINUE : FILE_NOT_FOUND);
}

t_redir_status			redirections_handler(t_exec** info, t_bst* cmd, t_term* term, char*** filename)
{
	int		            tmp;
	int					height;
    t_redir_status      redir_st;

	tmp = -1; // TODO: Init
	if (!cmd->b)
		return (CONTINUE);
	if (!(*filename = tokens_expand((t_tok**)&cmd->b, &term->env, &height))) // TODO
		return (RDR_BAD_ALLOC);
	if ((redir_st = try_catch_filename(filename, ((t_tok*)cmd->b)->data, height)) != CONTINUE)
        return (redir_st);
	if (cmd->type & REDIR_GR || cmd->type & REDIR_DG)
		redir_st = try_catch_out(info, cmd->type, (*filename)[0]);
	else if (cmd->type & REDIR_DG)
		redir_st = try_catch_in(info, cmd->type, (*filename)[0]);
	return (redir_st);
}
