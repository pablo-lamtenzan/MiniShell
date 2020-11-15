/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 11:47:42 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>


static pid_t		find_pid(t_process* suspended, char* str_pid)
{
    pid_t           pid;
    int             i;
    t_process*      cp;

    i = -1;
    while (str_pid[++i])
        if (!ft_isdigit(str_pid[i]))
            return (false);
    pid = ft_atoi(str_pid);
    cp = suspended;
    while (cp)
    {
        if (cp->pid == pid)
            return (pid);
        cp = (t_process*)cp->data;
    }
    return (false);
}

int		ft_fg(t_exec* args, t_term* term)
{
    pid_t			pid;
    void*			addr;
    int				i;

    pid = term->suspended_processes->pid;
    // for the momment ignore other args
    if (args->ac > 1)
    {
        // check for this fct thes the true fg is strange
        if (!(pid = find_pid(term->suspended_processes, args->av[1])))
        {
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job", args->av[1]);
            return (STD_ERROR);
        }
    }
    kill(pid, SIGCONT);
    while (waitpid(pid, &term->suspended_processes->wstatus, 0) <= 0)
        ;
    if (!is_suspended(term->suspended_processes->wstatus))
        remove_suspended_process(&term->suspended_processes);
    return (SUCCESS);
}