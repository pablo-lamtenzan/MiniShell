/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 09:31:51 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <lexer/lexer.h>
#include <execution.h>
#include <separators.h>
#include <builtins.h>
#include <signals.h>
#include <string.h>

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

static void			handle_exec_error(t_bst* root, t_exec_status exec_st)
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
	session_end();
	exit(exit_val);
}

// TO DO: Norme

// Kill -l cross-plateform
// Excution some non senses
// Perhabs redo the fct

void	exec(t_tok* tokens)
{
	t_exec_status	exec_st;
	int				flags[3];
	t_tok			*exec_tokens;
	t_bst			*root;

	ft_bzero(flags, sizeof(flags));
	while ((exec_tokens = handle_separators(&tokens, &flags[STATUS], &flags[PARETHESES_NB])))
	{
		if (handle_conditionals(flags[STATUS], &flags[CONDITIONALS], flags[PARETHESES_NB]))
		{
			if ((exec_st = execute_bst(root = bst(exec_tokens))) != SUCCESS)
				handle_exec_error(root, exec_st);
			free_bst(root);
		}
	}
}

void	test(int signal)
{
	(void)signal;
	ft_dprintf(2, "THIS IS A TEST\n");
}

static bool				init(int ac, const char **av, const char **ep)
{
	if (ac > 0 && session_start())
	{
		if ((g_session.name = ft_basename(av[0])))
		{
			if ((g_session.env = env_import(ep)))
			{
				if (term_init(&g_session.env))
					return (true);
				env_clr(&g_session.env);
			}			
			free(g_session.name);
		}
		session_end();
	}
	return (false);
}

void					lex_reset(t_lex_st *st)
{
	token_clr(&st->tokens);
	st->input = NULL;
	st->wait = TOK_NONE;
	st->subshell_level = 0;
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
	g_session.st = 258;
	lex_reset(st);
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.pos = 0;
	g_term.pos = 0;
}



int						main(int ac, const char **av, const char **ep)
{
	static const char*	seps[4] = {"||", "&&", ";", NULL};
	t_term_err			term_status;
	t_lex_err			lex_status;
	t_lex_st			lex_data;

	init_signal_handler();
	if (!init(ac, av, ep))
		return (1);
	ft_bzero(&lex_data, sizeof(lex_data));
	term_status = TERM_EOK;
	lex_status = LEX_EOK;
	while ((g_term.msg = string_expand(TERM_PS1, g_session.env))
	&& (term_status = term_prompt(&lex_data.input)) == TERM_ENL)
	{
		if ((lex_status = lex_tokens(&lex_data)) == LEX_EOK)
		{
			g_session.input_line_index = 0;
			delete_input_line();
			g_session.input_line = split_separators(g_term.line->data, seps);
			exec(lex_data.tokens);
			lex_data.tokens = NULL;
		}
		else if (lex_status == LEX_EWAIT)
			term_write(TERM_ENDL, sizeof(TERM_ENDL) - 1);
		else
			syntax_error(&lex_data);
		free(g_term.msg);
	}
	term_destroy();
	return (term_status);
}
