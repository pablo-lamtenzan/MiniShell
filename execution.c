/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 13:55:19 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/22 18:40:46 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bst.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

/* Have to add the libft for have the libc fcts */

/* Have to add the env (char **) and the return value $?
*/
bool				is_builting(const int ac, char **argv, t_data *data)
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
		data->return_status = ft_env(data);
	else if (!ft_strncmp(name, "exit", 5))
		data->return_status = ft_exit(data);
	else
		return (false);
	return (true);
}

/* Normally witouth redirecting the fd, dont need to fork */
void			execute_simple_cmd(char *simple_cmd, t_data *data)
{
	char		*execution_path;
	int			ret_status; // need to be in global struct
	char		**envp; // env needed for execve, have to do it too

	/* NEED A CHAR ** FOR EXECVE NOT A CHAR * */

	if (!is_builting(data->ac, simple_cmd))
	{
		/* Here need the path for execve */
		/* Here need to set up the envp */
		/* This variables could be const */
		ret_status = execve(execution_path, simple_cmd, envp);
		ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
		return (false);
	}
}

bool			open_and_dup_fd(t_bst *head)
{
	int			fd;

	if (head->operator & REDIRECTION_GREATHER)
	{
		if ((fd = open(head->cmd[1], O_WRONLY | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
			return (false);
		dup2(fd, STDOUT_FILENO);
	}
	else if (head->operator & REDIRECTION_LESSER)
	{
		if ((fd = open(head->cmd[1], O_RDONLY)) < 0)
			return (false);
		dup2(fd, STDIN_FILENO);
	}
	else if (head->operator & REDIRECTION_DGREATHER)
	{
		if ((fd = open(head->cmd[1],  O_WRONLY | O_APPEND | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
			return (false);
		dup2(fd, STDOUT_FILENO);
	}
	if (close(fd) < 0)
		return (false);
	return (true);
}

bool			execute_redirections_cmd(t_bst *head, t_data *data)
{
	int			pid; // has to be in the global struct
	int			ret_status;
	char		**execution_path;
	char		**envp;

	/* NEED A CHAR ** FOR EXECVE NOT A CHAR * */

	if (!(pid = fork()))
	{
		if (!open_and_dup_fd(head))
			return (false);
		if (is_builting(0/*need the ac*/, head->cmd[0])) // problem char * incompatible with char ** see bst.h
		{
			ret_status = execve(execution_path, head->cmd[0]/*wrong*/, envp);
			ft_printf("HAVE TO CUSTOMIZE THIS ERROR MSG SMOOTHLY\n");
			return (false);
		}
		else if (pid < 0)
			return (!(ret_status = 127));
		while (waitpid(pid, NULL, 0) <= 0)
			;
		pid = 0;
		return (true);
	}

	bool		execute_pipe_cmd(t_bst *head, t_data *data)
	{
		/* TO DO, 
		ONLY DIFFICULT THINK IN THIS PROJECT:
		cmd | cmd | cmd or
		cmd | cmd | cmd > file
		*/
		return (true);
	}