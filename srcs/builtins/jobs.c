/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 14:41:04 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
// [nb suspended]['+' last / '-' last - 1 / ' ' others ][' '][pid][' '][Stopped][SIGNAL][espaces][args]

// more signals to add
char*			get_signal(int wstatus)
{
	const char*	signals[4] = {"SIGSTOP", "SIGSTPT", "SIGTTIN", "SIGTTOU"};
	return (signals[WTERMSIG(wstatus) - SIGSTOP]);
}

int				get_id(t_process* curr, t_process* history)
{
	if (history && curr->pid == history->pid)
		return (0);
	if (history && history->next && curr->pid == history->next->pid);
		return (1);
	return (2);
}

int				ft_jobs(t_exec* args, t_term* term)
{
    (void)args;
    const char* id[3] = { "+", "-", " " };
	void*		addr_pid;
	t_process*	cp;
	t_group*	groups;
	int			i;

	// + - -> "+" = last pid (ussed not added), "-" = last pid (ussed not added) -
	// value into parentheses cound be more than the values alreaddy defined
	groups = term->session->groups;
	while (groups)
	{
		cp = groups->nil->prev;
		while (cp != term->session->groups->nil)
		{
			// "Stopped field cound be another msg (to test)"
			// get signal can return more signals
			ft_dprintf(STD_ERROR, "[%d]%s %s Stopped (%s)                   ", \
					get_background_index(term->session->groups, cp), \
					id[get_id(cp, term->session->history)], addr_pid = ft_itoa(cp->pid), \
					get_signal(cp->wstatus));
			i = -1;
			while (cp->data && cp->data[++i])
				ft_dprintf(STD_ERROR, "%s%s", \
					cp->data[i], cp->data[i + 1] ? " " : "");
			write(2, "\n", 1);
			cp = cp->prev;
			free(addr_pid);
		}
		groups = groups->next;
	}
	// check for error values
	return (SUCCESS);
}