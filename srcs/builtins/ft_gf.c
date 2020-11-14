/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gf.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:57:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <errors.h>

int         ft_fg(t_exec* args, t_term* term)
{
    pid_t   pid;
    void*   addr;
    int     i;

    pid = term->suspended_processes->pid;
    // for the momment ignore other args
    if (args->ac > 1)
    {
        // check for this fct thes the true fg is strange
        if (!(pid = find_pid(term->suspended_processes, args->av[1])))
        {
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job", args->av[1]);
            return (CMD_NOT_FOUND);
        }
    }
    else
    {
        addr = term->suspended_processes;
        term->suspended_processes = term->suspended_processes->data;
        free(addr);
    }
    kill(pid, SIGCONT);
    return (SUCCESS);
}