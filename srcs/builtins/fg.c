/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 15:46:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>

static pid_t		find_pid(t_process* suspended, char* str_index)
{
    size_t          index;
    int             i;
    t_process*      cp;

    i = -1;
    if (!str_index)
        return (false);
    while (str_index[++i])
        if (!ft_isdigit(str_index[i]))
            return (false);
    index = ft_atoi(str_index);
    cp = suspended;
    while (cp && (index--) && index)
        cp = cp->next;
    if (index > 0)
        return (false);
    return (cp->pid);
}

int		ft_fg(t_exec* args, t_term* term)
{
    pid_t			pid;
    void*			addr;
    int				i;

    pid = term->suspended_processes->pid;
    if (args->ac > 1)
    {
        if ((*args->av[1] && *(args->av[1] + 1) =! '%') \
            || !(pid = find_pid(term->suspended_processes, args->av[1][0] ? &args->av[1][1] : NULL)))
        {
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job", args->av[1]);
            return (STD_ERROR);
        }
    }
    kill(pid, SIGCONT);
    while (waitpid(pid, &term->suspended_processes->wstatus, 0) <= 0)
        ;
    // probally better check if exited
    if (!is_suspended(term->suspended_processes->wstatus))
        remove_suspended_process(&term->suspended_processes);
    else
        update_used_pids(pid, &term->used_pids);
    return (SUCCESS);
}