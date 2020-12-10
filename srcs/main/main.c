/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/12/10 20:54:45 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <lexer/lexer.h>
#include <execution.h>
#include <separators.h>
#include <builtins.h>
#include <signals_print.h>
#include <job_control/utils.h>
#include <signal_handler.h>

#include <string.h>
#include <unistd.h>

// TODO: ERROR codes: execution and builtins [error msg] -> [optional help] -> [return]
/* Errors List:
- bash: a: command not found -> 127
- bash: syntax error near unexpected token '||' -> not return (token type could be anything) -> 2
- bash: syntax error near unexpected token `newline' -> input: "<" -> 2
- bash: .: filename argument required"\n".: usage: . filename [arguments] -> input "." (called bash period) -> 2
- bash: a: No such file or directory -> input: echo < a -> 1
- bash: $a: ambiguous redirect -> 1
- bash:	filename: File name too long -> redirect to a long filename -> 1

// perror for fork?
// SIGPIPE for pipes
// SIGHUP job control (sent to a job when its term is closed 99% sure)

// SIGHUP dans le term -> pour chaque process (actif ou stoppe) SIGHUP -> ensuite pour chauqe process stoppe SIGCONT -> ensuite exit
// Cat ; ctrl + z -> SIGSTPT -> if (signal stoped and read or writes) -> all group process recives SIGTTIN

void	token_print(t_tok *tokens, const char *prefix)
{
	ft_dprintf(2, "[%s]\n", prefix);
	while (tokens)
	{
		if (tokens->type & TOK_CMD)
			token_print(tokens->data, "CMD");
		else if (tokens->type == TOK_PARAM)
			token_print(tokens->data, "PRM");
		else
			ft_dprintf(2, "[%s][%5hu] '%s'\n", prefix, tokens->type, tokens->data);
		tokens = tokens->next;
	}
}
*/

static void			handle_exec_error(t_bst *root, t_exec_status exec_st)
{
	// TO DO: TEST the display of thease error msg
	static const char*		labels[] = {
		"malloc",
		"close",
		"pipe",
		"dup2",
		"fork" // probably never used
	};
	const int				exit_vals[] = {
		SIGNAL_BASE + SIGABRT, 
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS
	};
	int						exit_val;

	if (exec_st < sizeof(labels) / sizeof(*labels))
	{
		exit_val = exit_vals[exec_st];
		ft_dprintf(STDERR_FILENO, "[%d] %s: %s: %s\n",
			exit_val, g_session.name, labels[exec_st], strerror(errno));
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

// TO DO: sleep 22 ; ctrl^Z ; bg ; fg ; ctrl^C <----------- NEED \n after ^C in this case
// cd leak

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
	}
	strs_unload(g_session.input_line);
	g_session.input_line = NULL;
}

/* static bool				init_login()
{
	// Load rc files
	return (true);
}

static bool				init_interactive()
{
	// Bind interactive signal handlers
	// Init environment variables (DIRNAME, PS1, PS2, etc...)
	// Load history
	return (true);
} */

static bool				init(const char *name, const char **ep)
{
	bool	is_login;

	if ((is_login = (*name == '-')))
		name++;
	if (session_start(&g_session, name, ep))
	{
		if (term_init(&g_session.env, g_session.cwd))
		{
			init_signal_handler(g_term.is_interactive && !g_term.has_caps);
			return (true);
		}
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

static t_line	*msg_get(t_lex_err status)
{
	const char* const	src =
		env_get(g_session.env, (status == LEX_EWAIT) ? "PS2" : "PS1", 3);

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
	while ((!g_term.is_interactive || (g_term.msg = msg_get(lex_status)))
	&& (status = term_prompt(&lex_data.input)) == TERM_ENL)
	{
		if ((lex_status = lex_tokens(&tokens, &lex_data)) == LEX_EOK)
		{
			exec(&tokens);
			token_clr(&tokens);
		}
		else if (lex_status != LEX_EWAIT)
		{
			syntax_error(&lex_data);
			token_clr(&tokens);
		}
		line_clear(&g_term.msg);
	}
	return ((g_term.is_interactive && !g_term.msg) ? TERM_EALLOC : status);
}

int						main(int ac, const char **av, const char **ep)
{
	t_term_err	status;

	if (ac < 1 || !init(av[0], ep))
		return (1);
	status = routine();
	session_end(&g_session);
	term_destroy();
	return (status == TERM_EEOF ? g_session.st : status);
}
