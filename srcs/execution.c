/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/06 17:37:29 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <minishell.h> // rm this when i could include without error builtins.h
//#include <builtins.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <path.h>

bool				is_builtin(int ac, char* *argv, t_term *term)
{
	const char		*name = argv[0];

	if (!ft_strncmp(name, "echo", 5))
		term->st = ft_echo(ac, argv);
	else if (!ft_strncmp(name, "cd", 3))
		term->st = ft_cd(ac, argv, term);
	else if (!ft_strncmp(name, "pwd", 4))
		term->st = ft_pwd();
	else if (!ft_strncmp(name, "export", 7))
		term->st = ft_export(ac, argv, term);
	else if (!ft_strncmp(name, "unset", 6))
		term->st = ft_unset(ac, argv, term);
	else if (!ft_strncmp(name, "env", 4))
		term->st = ft_env(term->env); // should ve a t_map
	else if (!ft_strncmp(name, "exit", 5))
		term->st = ft_exit(term);
	else
		return (false);
	ft_printf("term status after builting is %d\n", term->st);
	return (true);
}

bool				get_path_and_envp(char **execution_path, char***envp, char *cmd_name, t_term *term)
{
	if (!(*execution_path = path_get(cmd_name, map_get(term->env, "PATH")->value)) && !(term->st = 127))
	{
		ft_dprintf(2, "path not found! name is [%s]\n", cmd_name);
		return (false);
	}
	if (!(*envp = map_export(term->env)))
	{
		ft_dprintf(2, "could not export environment!");
		free(execution_path);
		return (false);
	}
	return (true);
}

/* I'm modifying data->envp, probally a bad idea TO SEE LATER*/

/* Normally witouth redirecting the fd, dont need to fork */
bool				execute_simple_cmd(t_bst *curr, t_term *term) // tokenize -> token -> 
{
	char	*execution_path = NULL;
	char*	*envp = NULL;

	if (!is_builtin(curr->ac[0], curr->av[0], term))
	{
		if (!(term->pid = fork()))
		{
			if (!(get_path_and_envp(&execution_path, &envp, *curr->av[0], term)))
				return (free_bst_node(&curr));
			term->st = execve(execution_path, curr->av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
		else if (term->pid < 0)
			return (!(term->st = 127));
		while (waitpid(term->pid, NULL, 0) <= 0)
			;
		term->pid = 0;
		free_ptrs_and_bst(execution_path, envp, NULL);
		// have to free curr ?
		return (true);
	}
	return (true);
}

static bool			open_and_dup_stdio(t_bst *curr)
{
	int				fd;

	fd = -1;
	if (curr->operator & REDIR_GR)
	{
		if ((fd = open(curr->av[1][0], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIR_LE)
	{
		if ((fd = open(curr->av[1][0], O_RDONLY)) < 0 \
				|| dup2(fd, STDIN_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIR_DG)
	{
		if ((fd = open(curr->av[1][0],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	return (fd > 0 && !(close(fd) < 0));
}

bool				execute_redirections_cmd(t_bst *curr, t_term *term)
{
	char	*execution_path = NULL;
	char	**envp = NULL;

	if (!(term->pid = fork()))
	{
		if (!open_and_dup_stdio(curr))
			return (false);
		if (!is_builtin(curr->ac[0], curr->av[0], term))
		{
			if (!(get_path_and_envp(&execution_path, &envp, *curr->av[0], term)))
				return (free_bst_node(&curr));
			term->st = execve(execution_path, curr->av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
	}
	else if (term->pid < 0)
		return (!(term->st = 127));
	while (waitpid(term->pid, NULL, 0) <= 0)
		;
	term->pid = 0;
	free_ptrs_and_bst((void*)execution_path, (void*)envp, NULL);
	// have to free curr ?
	return (true);
}
