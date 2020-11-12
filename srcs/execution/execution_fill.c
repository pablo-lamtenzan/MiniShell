/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 20:05:45 by pablo             #+#    #+#             */
/*   Updated: 2020/11/12 00:56:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h> // to change
#include <path.h>

int     	handle_wstatus(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	return (wstatus);
}

int			execute_child(t_exec* info, t_term* term)
{
	int		wstatus;

	if (!(term->pid = fork()))
	{
		if (dup_stdio(info->fds, &info->handle_dup))
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
	return (handle_wstatus(wstatus));
}

bool		build_execve_args(t_exec** info, t_term* term)
{

	if (!((*info)->execution_path = path_get((*info)->av[0], env_get(term->env, "PATH"))))
		return (!(term->st = 127));
	if (!((*info)->ep = (char*const*)env_export(term->env)))
	{
		free((void*)(*info)->execution_path);
		return (false);
	}
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

char**		handle_return_status(char** av, t_term* term)
{
	int		i;
	static char** to_free = NULL;

	if (to_free)
	{
		free(to_free);
		to_free = NULL;
	}
	i = -1;
	while (av[++i])
		if (!ft_strncmp("$?", av[i], 2))
		{
			av[i] = ft_itoa(term->st);
			to_free = &av[i]; // this stuff in a struct and free it
		}
	return (av);
}

int	tkt_size(t_tok *lst)
{
	int		size;

	if (!lst)
		return (0);
	size = 1;
	while ((lst = lst->next))
		size++;
	return (size);
}

void		ft_strpcy(char* dest, char* src)
{
	int i;

	i = -1;
	while (src[++i])
		dest[i] = src[i];
}

bool		temporally_expansion(t_tok* args, char*** av, t_term* term)
{
	int		i;
	size_t	size;
	(void)term;

	i = 0;
	if (!(*av = malloc(sizeof(char*) * (size = tkt_size(args)))))
		return (false);
	(*av)[size] = 0;
	while (args)
	{
		(*av)[i] = ft_strdup(args->data);
		i++;
		args = args->next;
	}
	return (true);
}