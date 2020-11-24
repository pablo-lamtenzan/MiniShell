/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 03:11:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/24 11:24:19 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <libft.h>

// do ./minishell, then ./a.out, then ctrl+D and its works

void			zombies_catcher(int signal)
{
	(void)signal;

	t_group*	remember;
	t_process*	remember_leader;
	t_group*	next_g;
	t_process*	next_p;
	int			wret;

	if (!g_session)
		return ;
	
	remember = g_session->groups;

	while (g_session->groups != g_session->nil)
	{
		remember_leader = g_session->groups->active_processes;
		next_g = g_session->groups->next;
		while (g_session->groups->active_processes != g_session->groups->nil)
		{
			next_p = g_session->groups->active_processes->next;
			if (g_session->groups->active_processes->flags & BACKGROUND)
			{
				while (42)
				{
					wret = waitpid(g_session->groups->active_processes->pid, &g_session->groups->active_processes->wstatus, WNOHANG | WUNTRACED);
					if (wret == -1)
					{
						if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER FAIL (error)][\'%p\']\n", g_session->groups->active_processes);
						break ;
					}
					else if (wret == 0)
					{
						if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER DOESN'T CACTH A CHANGE IN][\'%p\'] STATE\n", g_session->groups->active_processes);
						break ;
					}
					else
					{
						// TO DO: DOES NOT DISPLAY PROMT (research about)
						if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER SUCESS][\'%p\']\n", g_session->groups->active_processes);
						g_session->groups->active_processes->flags &= ~BACKGROUND; // for jobs printing
						// Catch return
						if (WIFEXITED(g_session->groups->active_processes->wstatus))
						{
							if (PRINT_DEBUG)
								ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' EXITS][exit return: %d]\n", g_session->groups->active_processes, WEXITSTATUS(g_session->groups->active_processes->wstatus));
							g_session->st = WEXITSTATUS(g_session->groups->active_processes->wstatus);
							g_session->groups->active_processes->flags |= EXITED; // print too
						}
						else if (WIFSIGNALED(g_session->groups->active_processes->wstatus))
						{
							if (PRINT_DEBUG)
								ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' IS SIGNALED]\n", g_session->groups->active_processes);
							g_session->st = WTERMSIG(g_session->groups->active_processes->wstatus) + SIGNAL_BASE;
							//g_session->groups->active_processes->flags |= EXITED; // not sure if it happens in all the case
						}
						else
						{
							if (PRINT_DEBUG)
								ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' STOP]\n", g_session->groups->active_processes);
							g_session->groups->active_processes->flags |= STOPPED;
							g_session->st = 148; // stopped st
						}

						break ;
					}
				}
			}
			if (g_session->groups)
				g_session->groups->active_processes = next_p;
			else
				break ;
		}
		if (g_session->groups)
			g_session->groups->active_processes = remember_leader;
		g_session->groups = next_g;
	}
	g_session->groups = remember;
}