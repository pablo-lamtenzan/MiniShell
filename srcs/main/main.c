/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 21:55:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <separators.h>
#include <builtins.h>

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

static void			handle_exec_error(t_bst* root, t_exec_status exec_st, t_term* term)
{
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
	// TODO: resume_suspended_processes(&term->suspended_processes);
	term_destroy(term);
	tputs(term->caps.insert_end, 0, &ft_putchar);
	write(STDERR_FILENO, "exit\n", 5);
	exit(exit_return[exec_st]);// TO DO: jobs witout -l no spaces between pipes and cmds

}

// TO DO: [OPTIONAL] put color in the prompt

// TO DO: Norme all (at the end of the end)
// TO DO: [UNKNOWN] cat | cat -e | echo a resarch (now we have all the job control build, must be easy to fix)

// DATA-STRUCTURE PROBLEMS:
// TO DO: [KILL] Update the data-structure well with SIGNALED flags
// TO DO: [BG] if current is running bg has problems ... (not sure)
// TO DO: Whan a group end in background print it????? (in bash group isn t in history after) <--- HISTORY

// TO TEST
// TO DO: [PIPES WITH STOPPED] return (g_session->st) last pipe process ret when theres stopped ? (check things like that for bg, kill...)
// TO DO: [PRINT] must print exit status if its != 0

// BACKGROUND GROUP RETURN
// ---> Stopped group return the signal? (128) + last NOT stopped (ret) (last before 1st stop) DONE
	// not last ? so the signal value of the stopped
// ---> When the process ends its printed but the return after bg is 0 always theres no problems [BG RETURN 0][LAST IS PRINTED] DONE

// FOREGROUND GROUP RETURN
// ---> Stopped group return the signal? (128) + last NOT stopped (ret) (last before 1st stop) DONE
	// not last ? so the signal value of the stopped
// ---> Whan the process ends RETURNS THE LAST IN THE GROUP RETURN [FG RETURN][NOTHING PRINTED] DONE

// UNWORKING STUFF I FOUND
// echo $? doesnt work


static int 			exec(t_tok* tokens, t_term* term)
{
	t_exec_status	exec_st;
	int				flags[3];
	t_tok*			exec_tokens;
	t_bst*			root;

	ft_bzero(flags, sizeof(flags));
	while ((exec_tokens = handle_separators(&tokens, &flags[STATUS], &flags[PARETHESES_NB])))
	{
		if (handle_conditionals(&term, flags[STATUS], &flags[CONDITIONALS], flags[PARETHESES_NB]))
		{
			if ((exec_st = execute_bst(root = bst(exec_tokens), term)) != SUCCESS)
				handle_exec_error(root, exec_st, term);
			free_bst(root);
		}
	}
	return (0);
}

void	suspend_process(int signal)
{
	(void)signal;

	write(2, "\n", 1);
	if (g_session->open_print)
		print_signal(2, g_session->groups->active_processes, 0);
	
}

void	test(int signal)
{
	(void)signal;
	ft_dprintf(2, "THIS IS A TEST\n");
}

int		main(int ac, const char** av, const char** envp)
{
	signal(SIGTSTP, suspend_process);
	signal(SIGCHLD, zombie_catcher_v2);
	//signal(SIGTTIN, test);
	//signal(SIGTERM, todo); // need documentation about this
    return (term_prompt(ac, av, envp, &exec));
}
