/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 08:03:45 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>

#include <process.h>
#include <signals.h>

static int	handle_wstatus(int wstatus, char*const* av)
{
	int		i;

	i = -1;
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
	{
			print_signals(wstatus, (const char**)av);
			wstatus += 128;
	}
	return (wstatus);
}

bool        wait_processes(t_process* processes, int* term_st)
{
    int     i;

    i = 0;
    while (++i < processes[MANAGE].pid)
    {
        while (waitpid(processes[i].pid, &processes[i].wstatus, 0) <= 0)
            ;
        *term_st = handle_wstatus(&processes[i].wstatus, &processes[i].data);
    }
	return (true);
}