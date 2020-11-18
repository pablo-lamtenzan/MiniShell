/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 23:11:42 by pablo             #+#    #+#             */
/*   Updated: 2020/11/19 00:51:54 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>

int		ft_bg(t_exec* args, t_term* term)
{
    t_process**		target;

    if (!term->session->groups || term->session->groups == term->session->nil \
			|| !term->session->groups->active_processes \
			|| term->session->groups->active_processes == term->session->groups->nil)
    {
        ft_dprintf(STD_ERROR, "minish: bg: current: no such job\n");
        return (STD_ERROR);
    }

    target = &term->session->groups->active_processes;
    if (args->ac > 1)
    {
        if (!(target = jobspec_parser(term->session, args->ac, args->av, NULL)))
		{
            ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
    kill((*target)->pid, SIGCONT);
    //update_background(term->session, target);
	// I haven't to wait but i need the status to know when it finishs (TO THINK ABOUT)
	// Can use SIGHILD to know when it ends
	// https://softwareengineering.stackexchange.com/questions/162940/how-do-i-implement-the-bg-and-fg-commands-functionaliity-in-my-custom-unix-s
	// https://stackoverflow.com/questions/7171722/how-can-i-handle-sigchld (doesn t respect the subject but very interesting)
	// https://docs.oracle.com/cd/E19455-01/806-4750/signals-7/index.html (this follows the subject)

	// can use command ps ax for get the pid table, then i can split it by ? and rm the i % 2 != 0 of the split
	/*

	signal(SIGCHLD, handle_zombies);

	void		handle_zombies(int signal)
		fork()
		execve(ps ax > /dev/null)
		read from /dev/nul
		split(read)
		int i = 0;
		while (lenght)
		{
			if (i % 2 == 0)
				check_pid();
				if (pid == background_running_process)
				{
					while (42)
					{
						union wait wstat
						int wstatus
						waitpid(pid, &wstatus, WNOHANG)
						or
						wait3(&wstat, HNOHANG, (struct rusage*)NULL);
						if (pid == 0)
							return;
						if pid == -1
							error
						if pid
							wstat.retcode -------> error status
					}
				}
		}
	*\
    return (SUCCESS);
}