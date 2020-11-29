/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 01:17:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <lexer/lexer.h>
#include <execution.h>
#include <separators.h>
#include <builtins.h>
#include <signals.h>

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
	const char*		error_msg[5] = {
		"[%d] minish: Fatal error: Not enought memory room, can't allocate memory blocks\n"
		"[%d] minish: Fatal error: Syscall: close: invalid return\n"
		"[%d] minish: Fatal error: Syscall: pipe: invalid return\n"
		"[%d] minish: Fatal error: Syscall: dup2: invalid return\n"
		"[%d] minish: Fatal error: Syscall: fork: invalid return\n" // fork probally never used
	};
	const int		exit_return[5] = {
		SIGNAL_BASE + SIGABRT, 
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS,
		SIGNAL_BASE + SIGSYS
	};
	ft_dprintf(STDERR_FILENO, error_msg[exec_st], exit_return[exec_st]);
	free_bst(root);
	term_destroy();
	session_end();
	exit(exit_return[exec_st]);
}

// TO DO: endzombies history (should not print)
// TO DO: do fork for builtins too but empty forks to call SIGCHID if (fork == 0){exit(builting ret value)}
// TO DO: bg + jobs prints 2 times (mute in jobs the exited nodes in the zombie catcher)
// TO DO: continious bg % LIKE KILL


// TO DO: [CHILL] kill by index so far kills the last instead of return error
// TO DO: [CHILL] Zombie cattcher doest rm the last zombie (sleep 2 , sleep 3, bg, bg, jobs)
	// Test before to call SIGCHLD for every cmd
	


// TO DO: [UNKNOWN] cat | cat -e | echo a resarch (now we have all the job control build, must be easy to fix)

// UNWORKING STUFF I FOUND
// echo $? doesnt work


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

void	suspend_process(int signal)
{
	(void)signal;

	write(STDERR_FILENO, "\n", 1);
	if (g_session->flags & OPEN_PRINT)
		print_signal(STDERR_FILENO, g_session->groups->active_processes, STANDART);
	
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
		if ((g_session->name = ft_basename(av[0])))
		{
			if ((g_session->env = env_import(ep)))
			{
				if (term_init(&g_session->env))
					return (true);
				env_clr(&g_session->env);
			}			
			free(g_session->name);
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
		g_session->name, input);
	g_session->st = 258;
	lex_reset(st);
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.pos = 0;
	g_term.pos = 0;
}

void					quit_process(int signal)
{
	(void)signal;
}

int						main(int ac, const char **av, const char **ep)
{
	static const char*	seps[4] = {"||", "&&", ";", NULL};
	t_term_err			term_status;
	t_lex_err			lex_status;
	t_lex_st			lex_data;

	signal(SIGTSTP, suspend_process);
	signal(SIGCHLD, zombie_catcher);
	signal(SIGQUIT, quit_process);
	//signal(SIGTTIN, test);
	//signal(SIGTERM, todo); // need documentation about this
	if (!init(ac, av, ep))
		return (1);
	ft_bzero(&lex_data, sizeof(lex_data));
	term_status = TERM_EOK;
	lex_status = LEX_EOK;
	while ((g_term.msg = string_expand(TERM_PS1, g_session->env))
	&& (term_status = term_prompt(&lex_data.input)) == TERM_ENL)
	{
		if ((lex_status = lex_tokens(&lex_data)) == LEX_EOK)
		{
			g_session->input_line_index = 0;
			g_session->input_line = split_separators(g_term.line->data, seps);
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
