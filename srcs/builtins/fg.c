/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/18 14:53:54 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>

// is is wrong (type)
t_process**	find_pid(t_session* session, char* str_index)
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
    ft_dprintf(2, "FG index= [%lu]\n", index);
    groups = session->groups;
    // for each group
    while (groups != session->nil)
    {    
        cp = groups->active_processes;
        // for each background process in each group
        while (cp != groups->nil && (index--) && index)
            cp = cp->next;
        groups = groups->next;
    }
    if (index > 0)
        return (NULL);
        // bad return
    return (NULL);
}

int		ft_fg(t_exec* args, t_term* term)
{
    t_process**		target;

    if (!term->session->groups || term->session->groups == term->session->nil || !term->session->groups->active_processes || term->session->groups->active_processes == term->session->groups->nil)
    {
        ft_dprintf(STD_ERROR, "bash: fg: current: no such job\n");
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
    ft_dprintf(2, "--------->%p\n", target);
    kill((*target)->pid, SIGCONT);
    // if TTIN or TTOUT -> SIGHUB (i suposse)
    update_background(term->session, target);
    return (SUCCESS);
}