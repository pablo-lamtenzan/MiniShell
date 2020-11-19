/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/18 23:10:40 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>

int		ft_fg(t_exec* args, t_term* term)
{
    t_process**		target;

    if (!term->session->groups || term->session->groups == term->session->nil \
			|| !term->session->groups->active_processes \
			|| term->session->groups->active_processes == term->session->groups->nil)
    {
        ft_dprintf(STD_ERROR, "minish: fg: current: no such job\n");
        return (STD_ERROR);
    }

    target = &term->session->groups->active_processes;
    if (args->ac > 1)
    {
        if (!(target = jobspec_parser(term->session, args->ac, args->av, NULL)))
		{
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
	ft_dprintf(2, "[FG][TARGET IS %p]\n", *target);
    kill((*target)->pid, SIGCONT);
    update_background(term->session, target);
    return (SUCCESS);
}