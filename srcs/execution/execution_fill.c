/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 02:45:43 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>
#include <string.h>
#include <sys/types.h>

int     	handle_wstatus(int wstatus, pid_t pid, char*const* av)
{
	int		i;

	i = -1;
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
	{
		if (wstatus == SIGSEGV)
		{
			ft_dprintf(2, "[%d]    %d %s  ", 1, pid, "segmentation fault (core dumped)");
			while (av[++i])
				ft_dprintf(2, "%s%c", av[i], av[i + 1] ? ' ' : 0);
			write(2, "\n", 1);
		}
		else
			ft_dprintf(2, "%s %d\n", strsignal(wstatus), wstatus);
	}
	return (wstatus);
}


int			execute_child(t_exec* info, t_term* term)
{
	int		wstatus;

	if (!(term->pid = fork()))
	{
		if (dup_stdio(info->fds))
		{
			wstatus = execve(info->execution_path, info->av, info->ep);
			ft_dprintf(2, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], wstatus);
		}
		exit(EXIT_FAILURE);
	}
	else if (term->pid < 0)
		return (errno);
	while (waitpid(term->pid, &wstatus, 0) <= 0)
		;
	return (handle_wstatus(wstatus, term->pid, info->av));
}

bool		build_execve_args(t_exec** info, t_term* term)
{
<<<<<<< HEAD
	ft_dprintf(2, "Goes into build_execve_args\n");
	if (!((*info)->execution_path = path_get((*info)->av[0], env_get(term->env, "PATH"))))
=======

	if (!((*info)->execution_path = path_get((*info)->av[0], env_get(term->env, "PATH", 4))))
>>>>>>> 708b446f09d84b7d6d62a255af6c61c80140722b
		return (!(term->st = 127));
	ft_dprintf(2, "[EXECUTION PATH][%s]\n", (*info)->execution_path);
	if (!((*info)->ep = (char*const*)env_export(term->env)))
	{
		free((void*)(*info)->execution_path);
		return (false);
	}
	ft_dprintf(2, "[ENV][%s]\n", *(*info)->ep);
	return (true);
}

void		destroy_execve_args(t_exec* info)
{
	char**	aux;
	int		i;

	aux = (char**)info->ep;
	i = 0;
	while (aux && aux[i++])
		free(aux);
	free(aux);
	free((void*)info->execution_path);
}

int			matrix_height(char*const* matrix)
{
	const char** it;

	it = (const char**)matrix;
	while (*it)
		it++;
	return ((char*const*)it - matrix);
}
