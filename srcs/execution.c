/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/02 17:24:47 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <minishell.h> // rm this when i could include without error builtins.h
//#include <builtins.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Have to add the libft for have the libc fcts */

size_t		token_count(t_token *tokens)
{
	size_t	count;

	count = 0;
	while (tokens)
	{
		//ft_printf("token count [%p]\n", tokens);
		count++;
		tokens = tokens->next;
	}
	return (count);
}

bool				is_builting(const int ac, const char **argv, t_data *data)
{
	const char		*name = argv[0];

	if (!ft_strncmp(name, "echo", 5))
		data->return_status = ft_echo(ac, argv);
	else if (!ft_strncmp(name, "cd", 3))
		data->return_status = ft_cd(ac, argv, data);
	else if (!ft_strncmp(name, "pwd", 4))
		data->return_status = ft_pwd();
	else if (!ft_strncmp(name, "export", 7))
		data->return_status = ft_export(ac, argv, data);
	else if (!ft_strncmp(name, "unset", 6))
		data->return_status = ft_unset(ac, argv, data);
	else if (!ft_strncmp(name, "env", 4))
		data->return_status = ft_env(data->env); // should ve a t_map
	else if (!ft_strncmp(name, "exit", 5))
		data->return_status = ft_exit(data);
	else
		return (false);
	return (true);
}

bool				get_path_and_envp(char **execution_path, char ***envp, char *cmd_name, t_data *data)
{
	if (!(*execution_path = path_get(cmd_name, map_get(data->env, "PATH")->value)))
		return (!(data->return_status = 127) \
					& ft_dprintf(2, "path not found! name is [%s]\n", cmd_name));
	return (!(*envp = map_export(data->env)) \
			&& ft_dprintf(2, "could not export environment!") \
			&& free_one_ptr(execution_path));
}

/* I'm modifying data->envp, probally a bad idea TO SEE LATER*/

/* Normally witouth redirecting the fd, dont need to fork */
bool				execute_simple_cmd(t_bst *curr, t_data *data) // tokenize -> token -> 
{
	char			*execution_path;
	char			**envp;

	if (!is_builting(curr->ac[0], curr->av[0], data))
	{
		if (!(get_path_and_envp(&execution_path, &envp, *curr->av[0], data)))
			return (free_bst_node(&curr));
		data->return_status = execve(execution_path, curr->av[0], envp);
		ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
		return (false);
	}
	free_four_ptrs_and_bst(NULL, &execution_path, &envp, NULL, &curr);
	return (true);
}

static bool			open_and_dup_stdio(t_bst *curr)
{
	int				fd;

	if (curr->operator & REDIRECTION_GREATHER)
	{
		if ((fd = open(curr->av[1], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIRECTION_LESSER)
	{
		if ((fd = open(curr->av[1], O_RDONLY)) < 0 \
				|| dup2(fd, STDIN_FILENO) < 0)
			return (false);
	}
	else if (curr->operator & REDIRECTION_DGREATHER)
	{
		if ((fd = open(curr->av[1],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	return (!(close(fd) < 0));
}

bool				execute_redirections_cmd(t_bst *curr, t_data *data)
{
	char			*execution_path;
	char			**envp;

	if (!(data->pid = fork()))
	{
		if (!open_and_dup_stdio(curr))
			return (false);
		if (!is_builting(curr->ac[0], curr->av[0], data))
		{
			if (!(get_path_and_envp(&execution_path, &envp, *curr->av[0], data)))
				return (free_bst_node(&curr));
			data->return_status = execve(execution_path, curr->av[0], envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
		else if (data->pid < 0)
			return (!(data->return_status = 127));
		while (waitpid(data->pid, NULL, 0) <= 0)
			;
		data->pid = 0;
		free_four_ptrs_and_bst(NULL, &execution_path, &envp, NULL, &curr);
		return (true);
	}
}


