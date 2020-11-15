/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 15:16:27 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
// [nb suspended]['+' last / '-' last - 1 / ' ' others ][' '][pid][' '][Stopped][SIGNAL][espaces][args]

char*			get_signal(int wstatus)
{
	const char*	signals[4] = {"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU"};
	return (signals[WTERMSIG(wstatus) - SIGSTOP]);
}

int				get_id(t_process* curr, pid_t* used_pids)
{
	if (curr->pid == used_pids[LAST])
		return (0);
	if (curr->pid == used_pids[PENULTIMATE])
		return (1);
	return (2);
}

int				ft_jobs(t_exec* args, t_term* term)
{
    (void)args;
    const char* id[3] = { "+", "-", " " };
	void*		addr_pid;
	t_process*	cp;
    size_t		count;
	int			i;

	count = suspended_process_nb(term->suspended_processes);
	cp = term->suspended_processes;
	while (cp->next)
		cp = cp->next;
	// + - -> "+" = last pid (ussed not added), "-" = last pid (ussed not added) -
	// value into parentheses cound be more than the values alreaddy defined
	while (cp)
	{
		ft_dprintf(STD_ERROR, "[%d]%s %s Stopped (%s)                   ", \
				count--, \
				id[get_id(cp, term->used_pids)], addr_pid = ft_itoa(cp->pid), \
				get_signal(cp->wstatus));
		i = -1;
		while (cp->data && cp->data[++i])
			ft_dprintf(STD_ERROR, "%s%s", \
				cp->data[i], cp->data[i + 1] ? " " : "");
		write(2, "\n", 1);
		cp = cp->prev;
		free(addr_pid);
	}
	return (SUCCESS);
}