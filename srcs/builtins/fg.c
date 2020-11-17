/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 14:35:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>

static t_process*	find_pid(t_session* session, char* str_index)
{
    size_t          index;
    int             i;
    t_process*      cp;
    t_group*        groups;

    i = -1;
    if (!str_index)
        return (NULL);
    while (str_index[++i])
        if (!ft_isdigit(str_index[i]))
            return (NULL);
    index = ft_atoi(str_index);
    // dbg
    ft_dprintf(2, "FG index= [%d]\n", index);
    groups = session->groups;
    // for each group
    while (groups)
    {    
        cp = groups->active_processes;
        // for each background process in each group
        while (cp != groups->nil && (index--) && index)
            cp = cp->next;
        groups = groups->next;
    }
    if (index > 0)
        return (NULL);
    return (cp);
}

int		ft_fg(t_exec* args, t_term* term)
{
    t_process*		target;
    void*			addr;
    int				i;

    target = term->session->groups->active_processes->pid;
    if (args->ac > 1)
    {
        if ((args->av[1][0] && args->av[1][1] != '%') \
            || !(target = find_pid(term->session, args->av[1][0] ? &args->av[1][1] : NULL)))
        {
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job", args->av[1]);
            return (STD_ERROR);
        }
    }
    kill(target->pid, SIGCONT);
    // if TTIN or TTOUT -> SIGHUB (i suposse)
    update_background(term->session, target);
    return (SUCCESS);
}