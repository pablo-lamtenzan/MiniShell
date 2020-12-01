/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 08:52:03 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 08:47:23 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

t_exec_status			print_redirection_error(t_redir_status rstatus,
		char **filename)
{
	const char			*error_msg[3] = {
		"minish: %s: No such file or directory\n",
		"minish: %s: ambigous redirect\n",
		"minish: %s: File name too long\n"
	};
	
	if (rstatus == RDR_BAD_ALLOC)
		return (RDR_BAD_ALLOC);
	ft_dprintf(STDERR_FILENO, error_msg[-rstatus - 1], filename);
	g_session.st = STD_ERROR;
	return (SUCCESS);
}

static t_redir_status	try_catch_filename(char ***filename, char *var_name,
		int height)
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

// TODO: Refactor
static t_redir_status	try_catch_in(t_exec **info, t_tok_t type,
		const char *filename)
{
	int					tmp;
	(void)type;

	// This is also the same function
	tmp = open(filename, O_RDONLY);
	if (!((*info)->handle_dup & REDIR_IN))
		(*info)->fds[FDS_STDIN] = tmp;
	(*info)->handle_dup |= REDIR_IN;
	return (tmp >= 0 ? CONTINUE : FILE_NOT_FOUND);
}


static t_redir_status	try_catch_out(t_exec** info, t_tok_t type, const char* filename)
{
	int	tmp;

	tmp = -1; // TODO: Init
	if (type & REDIR_GR)
	{
		// This is literally the same
		tmp = open(filename, O_WRONLY | O_CREAT | O_TRUNC, UMASK);
		if (!((*info)->handle_dup & REDIR_OUT))
			(*info)->fds[FDS_STDOUT] = tmp;
		(*info)->handle_dup |= REDIR_OUT;
	}
	else if (type & REDIR_DG)
	{
		// As this but with different options
		tmp = open(filename, O_WRONLY | O_CREAT | O_APPEND, UMASK);
		if (!((*info)->handle_dup & REDIR_OUT))
			(*info)->fds[FDS_STDOUT] = tmp;
		(*info)->handle_dup |= REDIR_OUT;
	}
	return (tmp >= 0 ? CONTINUE : FILE_NOT_FOUND);
}

t_redir_status			redirections_handler(t_exec** info, t_bst* cmd, char*** filename)
{
	int					height;
    t_redir_status      redir_st;
	char				*aux;

	if (!(t_tok*)cmd->b)
		return (CONTINUE);
	aux = (char*)((t_tok*)cmd->b)->data;
	if (!(*filename = tokens_expand((t_tok**)&cmd->b, &g_session.env, &height)))
		return (RDR_BAD_ALLOC);
	if ((redir_st = try_catch_filename(filename, aux, height)) != CONTINUE)
        return (redir_st);
	if (cmd->type & REDIR_GR || cmd->type & REDIR_DG)
		redir_st = try_catch_out(info, cmd->type, (*filename)[0]);
	else if (cmd->type & REDIR_DG)
		redir_st = try_catch_in(info, cmd->type, (*filename)[0]);
	return (redir_st);
}
