/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 07:46:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/24 22:58:55 by pablo            ###   ########.fr       */
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

// TO DO: [BUILTINS] Redirect fds in job control builtins
// TO DO: [KILL] Kill update background issue (can t wait fix it with a flags but i have problems now) I CAN'T WAIT IT IN KILL BUT I CAN WAIT AFTER IN FG WHY?????????????????????
// TO DO: [ALL BUILTINS] All builtins has error msg for pid and jobspec i have the same error msg...
// TO DO: [ALL BUILTINS] optimize builtins
// TO DO: [OPTIONAL] put color in the prompt
// TO DO: [KILL] Jobspec parser segmentation fault (reads NULL) -> sleep 22 + crtl^Z + sleep 22 + clrt^Z + kill -QUIT %sleep
// TO DO: [KILL] interaction between stopped and runnig processes and different kinds of SIGNALS
// TO DO: [ALL BUILTINS] Upgrade (better functions) and norme (shortter code) builtins
// TO DO: [UNKNOWN] cat | cat -e | echo a resarch (now we have all the job control build, must be easy to fix)
// TO DO: [PIPES WITH STOPPED] return (g_session->st) last pipe process ret when theres stopped ? (check things like that for bg, kill...)
// TO DO: [JOBS] must print exit status if its != 0
// TO DO: [JOBS] witout -l no spaces between pipes and cmds
// TO DO: [CTRL^Z] Print index + history if stopped job is not first (ctrl^Z)
// TO DO: [CTRL^Z] Print info when i: sleep 22 -> ctrl^Z -> bg -> fg -> ctrl^Z (in the second ctrl^Z)
// TO DO: [JOBS] print keeps "running" for finisheed background processes
// TO DO: [ALL BUILTINS]: fg, bg, jobs no pid, wait, kill, disown pid (easy cause i ve implemented this for all the builtins)


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
		// maybe it has to send SIGHUB to all the no market child (market in diswon)
    return (term_prompt(ac, av, envp, &exec));
}
