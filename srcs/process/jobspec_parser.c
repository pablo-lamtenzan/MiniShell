/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 12:40:22 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 17:53:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <process.h>
# include <libft.h>

/* UTILS */

int			matrix_height(char **matrix)
{
	char** it;

	it = matrix;
	while (*it)
		it++;
	return (it - matrix);
}

bool			is_string_digit(const char* string)
{
	int			i;

	i = -1;
	while (string[++i])
		if (string[i] > '9' || string[i] < '0')
			return (false);
	return (true);
}

bool			is_jobspec(const char* string)
{
	return (*string == '%');
}

bool			is_history_process(const char* string)
{
	return (string[0] == '%' && (!string[1] || (!string[2] && (string[1] == '+' || string[1] == '-' || string[1] == '%'))));
}

size_t			get_history_index(const char* key)
{
	if (key[0] == '+' || key[0] == '%' || key[0] == '\0')
		return (1);
	if (key[0] == '-')
		return (2);
	else
		return (0);
}

size_t			get_search_mode(const char* av)
{
	if (av[0] && av[0] != '?')
		return (1);
	if (av[0] == '?' && av[1])
		return (2);
	else
		return (0);
}

/* SEARCH FCTS */

// Remember to check for background spec &

// Returns group learder
t_process**		get_process_by_index(t_session* session, t_group* groups, size_t index)
{
	// %index
	if (!index)
		return (NULL);

	index++; // skip itself
	ft_dprintf(2, "[INDEX][index: %lu]\n", index);
	while (index && groups != session->nil)
	{
		//if (!--index)
		//	break ;
		index--;
		//while (groups->active_processes != groups->nil)
		//	groups->active_processes = groups->active_processes->next;
		groups = groups->next;
	}
	//ft_dprintf(2, "pid found: %d\n", groups->active_processes->pid);
	ft_dprintf(2, "[INDEX: %lu]\n", index);
	return (index ? NULL : &groups->nil->next);
}

// Returns addr in active_processes
t_process**		get_process_by_pid(t_session* session, t_group* groups, pid_t pid)
{
	// pid
	ft_dprintf(2, "[PID][pid: %i]\n", pid);
	while (groups != session->nil)
	{
		while (groups->active_processes != groups->nil)
		{
			if (groups->active_processes->pid == pid)
				return (&groups->active_processes);
			groups->active_processes = groups->active_processes->next;
		}
		groups = groups->next;
	}
	return (NULL);
}

// Returns addr in active processes
t_process**		get_process_by_name(t_session* session, t_group* groups, const char* av)
{
	// %name
	// %?name
	ft_dprintf(2, "[NAME][name: %s]\n", av);
	const int	search_mode = get_search_mode(&av[1]); // lexer
	int			match;
	t_process**	ret;
	int			count;

	if (search_mode == 0)
		return (NULL);
	match = 0;
	while (groups != session->nil)
	{
		while (groups->active_processes != groups->nil)
		{
			count = -1;
			if (search_mode == 1) // %name
			{
				ft_dprintf(2, "TEST  %s \n", &groups->active_processes->data[0][2]);
				if (!ft_strncmp(&groups->active_processes->data[0][2], av, ft_strlen(av)) && (match++)) // hange 1st arg to data[0] addr after trim ./ in execution fill
					ret = &groups->active_processes;
			}
			else if (search_mode == 2) // %?name
			{
				while (++count < matrix_height((char**)groups->active_processes->data)) // change matrix height later
					if (ft_strnstr(groups->active_processes->data[count], &av[1], ft_strlen(&av[1]) && (match++)))
						ret = &groups->active_processes;
			}
			groups->active_processes = groups->active_processes->next;
		}
		groups = groups->next;
	}
	if (match != 1)
		return (NULL); // display error ?
	return (ret);
}

// Returns group leader (IN HISTORY I ONLY HAVE THE LEADERS)
t_process**		get_process_by_history(t_session* session, t_group* groups, size_t index)
{
	// %
	// %%
	// %-
	// %+
	ft_dprintf(2, "[HISTORY][index: %lu]\n", index);
	t_process*	hist_addr;

	hist_addr = NULL;
	if (!session->history || !index)
		return (NULL);
	else if (!session->history->next || index == 1)
		hist_addr = session->history;
	else if (index == 2)
		hist_addr = session->history->next;
	return (get_process_by_pid(session, groups, hist_addr->pid));
}

t_process**		process_search(t_session* session, char*const* av)
{
	ft_dprintf(2, "[PROCESS SEARCH][AV IS [%s]]\n", av[1]);
	if (is_jobspec(av[1]))
	{
		if (is_string_digit(&av[1][1]))
			return (get_process_by_index(session, session->groups, ft_atoi(&av[1][1])));
		else if (is_history_process(av[1]))
			return (get_process_by_history(session, session->groups, get_history_index(&av[1][1])));
		else
			return (get_process_by_name(session, session->groups, &av[1][1]));
	}
	else if (is_string_digit(av[1]))
		return (get_process_by_pid(session, session->groups, ft_atoi(av[1])));
	else
		return (NULL);
}

t_process**		jobspec_parser(t_session* session, int ac, char*const* av, t_process** (*fill)(int ac, char*const* av))
{
	// TO DEFINE: types in fill
	ft_dprintf(2, "%s\n", av[1]);

	if (fill && !fill(ac, av))
		return (NULL);
	return (process_search(session, av));
}

// %index WORKS
// %pid WORKS
// %hist TO DGB (fg %, fg %- x2)
// %name/%?name TO DBG
