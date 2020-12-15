/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 22:37:32 by chamada           #+#    #+#             */
/*   Updated: 2020/12/14 22:37:33 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static const char	*exec_strerror(t_exec_status st, int *exit_val)
{
	static const char			*labels[] = {
		"malloc",
		"close",
		"pipe",
		"dup2",
		"fork"
	};
	static const unsigned char	exit_vals[] = {
		SIGNAL_BASE + SIGABRT,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS
	};

	if (--st >= 0 && st < sizeof(labels) / sizeof(*labels))
	{
		*exit_val = exit_vals[st];
		return (labels[st]);
	}
	*exit_val = st;
	return (NULL);
}

void				handle_exec_error(t_bst *root, t_exec_status exec_st)
{
	int	exit_val;

	ft_dprintf(STDERR_FILENO, "%s: %s: %s\n",
		g_session.name, exec_strerror(exec_st, &exit_val), strerror(errno));
	free_bst(root);
	term_destroy();
	session_end(&g_session);
	exit(exit_val);
}

void				syntax_error(t_lex_st *st)
{
	const char			*input;

	if (*st->input == '\n' || *st->input == '\0')
		input = "newline";
	else
		input = st->input;
	ft_dprintf(2, "%s: syntax error near unexpected token `%s'\n",
		g_session.name, input);
	g_session.st = STD_ERROR;
	ft_bzero(st, sizeof(*st));
}
