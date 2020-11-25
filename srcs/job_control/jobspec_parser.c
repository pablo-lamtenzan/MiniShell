/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 12:40:22 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 17:58:52 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <job_control.h>
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
	return (i);
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
	ft_dprintf(2, "AV : %s\n", av);
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
t_process**		get_process_by_index(t_group* groups, size_t index)
{
	// %index
	if (!index)
		return (NULL);

	//index++; // skip itself
	//if (PRINT_DEBUG)
	ft_dprintf(2, "[INDEX][index: %lu]\n", index);
	while (index && groups->next != g_session->nil)
	{
		//if (!--index)
		//	break ;
		index--;
		//while (groups->active_processes != groups->nil)
		//	groups->active_processes = groups->active_processes->next;
		groups = groups->next;
	}
	//ft_dprintf(2, "pid found: %d\n", groups->active_processes->pid);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[INDEX: %lu]\n", index);
	//ft_dprintf(2, "POSSIBLE ADDR TO RET: %p\n", groups);
	//ft_dprintf(2, "POSSIBLE ADDR TO RET: %p\n", &groups->nil->next);
	return (index ? NULL : &groups->nil->next);
}

// Returns addr in active_processes
/* THIS IS BASH IS IMPLEMEMNT LIKE PURE TRASH */
t_process**		get_process_by_pid(t_group* groups, pid_t pid)
{
	// pid
	if (PRINT_DEBUG)
		ft_dprintf(2, "[PID][pid: %i]\n", pid);
	t_process fill;

	fill.pid = pid;
	while (groups != g_session->nil)
	{
		if (background_find(&fill, "PID", groups))
			return (&groups->active_processes);
		groups = groups->next;
	}
	return (NULL);
}

// Returns addr in active processes
/* ONLY RETURNS IF IS LEADER */
t_process**		get_process_by_name(t_group* groups, const char* av)
{
	// %name
	// %?name
	ft_dprintf(2, "[NAME][name: %s]\n", av);
	const int	search_mode = get_search_mode(av); // lexer
	int			match;
	t_process**	ret;
	int			count;

	if (search_mode == 0)
		return (NULL);
	match = 0;
	ret = NULL;

	if (PRINT_DEBUG)
		ft_dprintf(2, "%p ------------ %p\n", g_session->groups, groups);
	while (groups != g_session->nil)
	{
		while (groups->active_processes != groups->nil)
		{
			count = -1;
			if (search_mode == 1) // %name
			{
				if (PRINT_DEBUG)
					ft_dprintf(2, "TEST  %s --- %s \n", groups->active_processes->data[0], av);
				if (!ft_strncmp(groups->active_processes->data[0], av, ft_strlen(groups->active_processes->data[0])) && (++match))
				{
					if (PRINT_DEBUG)
						ft_dprintf(2, "[DATA][FOUND: %p]\n", groups->active_processes);
					if (is_leader(groups->active_processes) && is_not_ambigous(groups->active_processes))
						return(&groups->active_processes);
				}
			}
			else if (search_mode == 2) // %?name
			{
				// TO DO: debug this
				while (++count < matrix_height((char**)groups->active_processes->data)) // change matrix height later
					if (ft_strnstr(groups->active_processes->data[count], &av[1], ft_strlen(&av[1]) && (++match)))
					{
						if (PRINT_DEBUG)
							ft_dprintf(2, "[DATA][FOUND: %p]\n", groups->active_processes);
						if (is_leader(groups->active_processes) && is_not_ambigous_v2(&av[1]))
						return(&groups->active_processes);
					}
			}
			groups->active_processes = groups->active_processes->next;
		}
		groups = groups->next;
	}
	ft_dprintf(2, "[DATA][NOT FOUND OR AMBIGOUS!!!!]\n");
	return (NULL);
}

// Returns group leader (IN HISTORY I ONLY HAVE THE LEADERS)
t_process**		get_process_by_history(t_group* groups, size_t index)
{
	// %
	// %%
	// %-
	// %+
	if (PRINT_DEBUG)
		ft_dprintf(2, "[HISTORY][index: %lu]\n", index);
	t_history*	hist_addr;

	hist_addr = NULL;
	if (!g_session->hist || !index)
		return (NULL);
	else if (!g_session->hist->next || index == 1)
		hist_addr = g_session->hist;
	else if (index == 2)
		hist_addr = g_session->hist->next;
	return (get_process_by_pid(groups, hist_addr->group->nil->next->pid));
}

t_process**		process_search(char*const* av)
{
	if (PRINT_DEBUG)
		ft_dprintf(2, "[PROCESS SEARCH][AV IS [%s]]\n", av[1]);
	if (is_jobspec(av[1]))
	{
		if (is_string_digit(&av[1][1]))
			return (get_process_by_index(g_session->groups, ft_atoi(&av[1][1])));
		else if (is_history_process(av[1]))
			return (get_process_by_history(g_session->groups, get_history_index(&av[1][1])));
		else
			return (get_process_by_name(g_session->groups, &av[1][1]));
	}
	else if (is_string_digit(av[1]))
		return (get_process_by_pid(g_session->groups, ft_atoi(av[1])));
	else
		return (NULL);
}

t_process**		jobspec_parser(int ac, char*const* av, bool (*fill)(int ac, char*const* av))
{
	// TO DEFINE: types in fill
	//if (PRINT_DEBUG)
		ft_dprintf(2, "%s\n", av[1]);

	if (fill && !fill(ac, av))
		return (NULL);
	return (process_search(av));
}

// %index WORKS
// %pid WORKS
// %hist TO DGB (fg %, fg %- x2)
// %name/%?name TO DBG
