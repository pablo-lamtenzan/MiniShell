/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/12/13 02:43:45 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <lexer/lexer.h>
#include <execution/execution.h>
#include <separators.h>
#include <builtins.h>
#include <signals_print.h>
#include <job_control/utils.h>
#include <signal_handler.h>

#include <string.h>
#include <unistd.h>

static void			handle_exec_error(t_bst *root, t_exec_status exec_st)
{
	static const char*		labels[] = {
		"malloc",
		"close",
		"pipe",
		"dup2",
		"fork"
	};
	static const int		exit_vals[] = {
		SIGNAL_BASE + SIGABRT,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS
	};
	int						exit_val;

	if (--exec_st >= 0 && exec_st < sizeof(labels) / sizeof(*labels))
	{
		exit_val = exit_vals[exec_st];
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n",
			g_session.name, labels[exec_st], strerror(errno));
	}
	else
	{
		exit_val = exec_st;
		ft_dprintf(STDERR_FILENO, "%s: unknown fatal error: %d!\n",
			g_session.name, exit_val);
	}
	free_bst(root);
	term_destroy();
	session_end(&g_session);
	exit(exit_val);
}

void	exec(t_tok **tokens)
{
	t_exec_status	exec_st;
	int				flags[3];
	t_tok			*exec_tokens;
	t_bst			*root;

	g_session.input_line_index = 0;
	g_session.input_line = split_separators(g_term.line->data);
	ft_bzero(flags, sizeof(flags));
	while ((exec_tokens = handle_separators(tokens, &flags[STATUS], &flags[PARETHESES_NB])))
	{
		if (handle_conditionals(flags[STATUS], &flags[CONDITIONALS], flags[PARETHESES_NB]))
		{
			if ((exec_st = execute_bst(root = bst(exec_tokens))) != SUCCESS)
				handle_exec_error(root, exec_st);
			free_bst(root);
		}
		else
			token_clr(&exec_tokens);
	}
	strs_unload(g_session.input_line);
	g_session.input_line = NULL;
}

static bool				init(const char *name, const char **ep)
{
	t_term_err	status;
	bool		is_login;

	if ((is_login = (*name == '-')))
		name++;
	if (session_start(&g_session, name, ep))
	{
		status = term_init(&g_session.env, g_session.cwd, is_login);
		if (status == TERM_EOK)
		{
			init_signal_handler();
			return (true);
		}
		term_perror(status);
		session_end(&g_session);
	}
	return (false);
}

void					syntax_error(t_lex_st *st)
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

static t_line	*msg_get(void)
{
	const char* const	src =
		env_get(g_session.env, "PS1", 3);

	return (src ? string_expand(src, g_session.env) : NULL);
}

static t_term_err	routine(void)
{
	t_term_err	status;
	t_lex_err	lex_status;
	t_lex_st	lex_data;
	t_tok		*tokens;

	tokens = NULL;
	ft_bzero(&lex_data, sizeof(lex_data));
	status = TERM_EOK;
	lex_status = LEX_EOK;
	while (lex_status != LEX_EALLOC
	&& (!g_term.is_interactive || (g_term.msg = msg_get()))
	&& (status = term_prompt(&lex_data.input)) == TERM_ENL)
	{
		if ((lex_status = lex_tokens(&tokens, &lex_data)) == LEX_EOK)
			exec(&tokens);
		else if (lex_status == LEX_ESYNTAX)
			syntax_error(&lex_data);
		if (tokens)
			token_clr(&tokens);
		line_clear(&g_term.msg);
	}
	return ((lex_status == LEX_EALLOC || (g_term.is_interactive && !g_term.msg))
		? TERM_EALLOC : status);
}

int						main(int ac, const char **av, const char **ep)
{
	t_term_err	status;

	if (ac < 1 || !init(av[0], ep))
		return (STD_ERROR);
	status = routine();
	session_end(&g_session);
	term_destroy();
	if (status != TERM_EEOF)
	{
		term_perror(status);
		return (STD_ERROR);
	}
	return (g_session.st);
}
