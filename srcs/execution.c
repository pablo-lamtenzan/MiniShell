/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/23 17:22:02 by plamtenz         ###   ########.fr       */
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

static bool			is_builting(const int ac, const char **argv, t_data *data)
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

/* I'm modifying data->envp, probally a bad idea TO SEE LATER*/

/* Normally witouth redirecting the fd, dont need to fork */
bool			execute_simple_cmd(t_bst *curr, t_data *data)
{
	char		*execution_path;
	char		**argv;
	char		**envp;
	int			ac;

	if (!(argv = token_tab(curr->cmd[0], &ac)))
		return (false);
	if (!is_builting(ac, argv, data))
	{
		if (!(execution_path = path_get(argv[0], \
				map_get(data->env, "PATH")->value)))
			return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", data->argv[0]) & \
					free_five_ptrs(&argv, NULL, NULL, NULL, NULL));
		if (!(envp = map_export(data->env)))
			return (ft_dprintf(2, "could not export environment!") & \
					free_five_ptrs(&argv, &execution_path, NULL, NULL, NULL));
		data->return_status = execve(execution_path, argv + 1, envp);
		ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
		return (false);
	}
	free_five_ptrs(&argv, &execution_path, &envp, NULL, NULL);
	return (true);
}

static bool		open_and_dup_fd(t_bst *head)
{
	int			fd;

	if (head->operator & REDIRECTION_GREATHER)
	{
		if ((fd = open(head->cmd[1], O_WRONLY | O_CREAT | O_TRUNC, \
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
					|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	else if (head->operator & REDIRECTION_LESSER)
	{
		if ((fd = open(head->cmd[1], O_RDONLY)) < 0 \
				|| dup2(fd, STDIN_FILENO) < 0)
			return (false);
	}
	else if (head->operator & REDIRECTION_DGREATHER)
	{
		if ((fd = open(head->cmd[1],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0 \
				|| dup2(fd, STDOUT_FILENO) < 0)
			return (false);
	}
	return (!(close(fd) < 0));
}

bool			execute_redirections_cmd(t_bst *curr, t_data *data)
{
	char		*execution_path;
	char		**argv;
	char		**envp;
	int			ac;

	if (!(argv = token_tab(curr->cmd[0], &ac)))
		return (false);

	if (!(data->pid = fork()))
	{
		if (!open_and_dup_fd(curr))
			return (false);
		if (!is_builting(ac, argv, data))
		{
			if (!(execution_path = path_get(argv[0], \
				map_get(data->env, "PATH")->value)))
				return (!(data->return_status = 127) & \
					ft_dprintf(2, "path not found! name is [%s]\n", data->argv[0]) & \
					free_five_ptrs(&argv, NULL, NULL, NULL, NULL));
			if (!(envp = map_export(data->env)))
				return (ft_dprintf(2, "could not export environment!") & \
				free_five_ptrs(&argv, &execution_path, NULL, NULL, NULL));
			data->return_status = execve(execution_path, argv + 1, envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
		else if (data->pid < 0)
			return (!(data->return_status = 127));
		while (waitpid(data->pid, NULL, 0) <= 0)
			;
		data->pid = 0;
		free_five_ptrs(&argv, &execution_path, &envp, NULL, NULL);
		return (true);
	}

	bool		execute_pipe_cmd(t_bst *curr, t_data *data)
	{
		;
		/* TO DO, 
		ONLY DIFFICULT THINK IN THIS PROJECT:
		cmd | cmd | cmd or
		cmd | cmd | cmd > file
		*/
		/* Architecture:
			1) pipe take stdout
			2) redirect stdout to stdin
			3) if redirection redirect stdout to file
			Repeat for each pipe
		*/
		return (true);
	}