/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 03:11:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 14:36:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <libft.h>

// do ./minishell, then ./a.out, then ctrl+D and its works

void			zombies_catcher(int signal)
{
	(void)signal;

	/*
	int wstatus;

	int ret;

	while (42)
	{
		// Can't wait pid cause if another child exist it will wait for the wrong interrumping minishell
		// THIS DOESN'T WORK FOR THE MOMMENT, NEED A PID (i guess)
		ret = waitpid(0, &wstatus, WNOHANG);
		if (!ret || ret == -1)
			return ;
		else
		{
			// STRANGE: returns 0 always (or it seems)
			int exit_st = WEXITSTATUS(wstatus);
			ft_dprintf(2, "CATCH ZOMBIE EXIT STATUS: %d\n", exit_st);
			// DO TO: need session global
			// term->st = WEXITSTATUS(wstatus);
			// DO TO:
				// Or rm each background node when i put the process in background
				// Or have HERE enought information to know which node delete
				// I must try with waitpid of my background processes:
		}
				 LIKE:
				
					while (my_backround_processes)
					{
						while (42)
						{
							int ret = waitpid("each backgound process", &wstatus, WNOHANG);
							if (!ret || ret == -1)
								break ;
							else
							{
								term->st = WIFEXITSTATUS(wstatus);
								update_background()
							}
						}
					}
					// AND HERE RM THE NOT ACTIVE GROUPS NOW
					while (all_my_groups) // groups are my stopped or running processes (running in foreground or background)
					{
						if (!is_active_group())
							// rm group
					}
					// If that works it will be so so nice
		}
		
	
		
	}
*/
	t_group*	remember;
	t_process*	remember_leader;
	t_group*	next_g;
	t_process*	next_p;
	int			wret;
	
	remember = g_session->groups;

	while (g_session->groups != g_session->nil)
	{
		remember_leader = g_session->groups->active_processes;
		next_g = g_session->groups->next;
		while (g_session->groups->active_processes != g_session->groups->nil)
		{
			next_p = g_session->groups->active_processes->next;
			if (g_session->groups->active_processes->flags & BACKGROUND && ft_dprintf(2, "[ZOMBIES CATCHER TRIES FOR: \'%p\'][pid =  %d]\n", g_session->groups->active_processes, g_session->groups->active_processes->pid))
			{
				while (42)
				{
					wret = waitpid(g_session->groups->active_processes->pid, &g_session->groups->active_processes->wstatus, WNOHANG | WUNTRACED);
					if (wret == -1)
					{
						ft_dprintf(2, "[ZOMBIES CATCHER FAIL][\'%p\']\n", g_session->groups->active_processes);
						break ;
					}
					else if (wret == 0)
					{
						ft_dprintf(2, "[ZOMBIES CATCHER FAIL][\'%p\']\n", g_session->groups->active_processes);
						break ;
					}
					else
					{
						// TO DO: DOES NOT DISPLAY PROMT (research about)
						ft_dprintf(2, "[ZOMBIES CATCHER SUCESS][\'%p\']\n", g_session->groups->active_processes);
						g_session->groups->active_processes->flags &= ~BACKGROUND; // for jobs printing
						// Catch return
						if (WIFEXITED(g_session->groups->active_processes->wstatus))
						{
							ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' EXITS][exit return: %d]\n", g_session->groups->active_processes, WEXITSTATUS(g_session->groups->active_processes->wstatus));
							; // g_session->st = WEXITSTATUS(g_session->groups->active_processes->wstatus); // TO DO st in session
							g_session->groups->active_processes->flags |= EXITED; // print too
						}
						else if (WIFSIGNALED(g_session->groups->active_processes->wstatus))
						{
							ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' IS SIGNALED]\n", g_session->groups->active_processes);
							; // g_session->st = WTERMSIG + SIGNAL_BASE;
							g_session->groups->active_processes->flags |= EXITED; // not sure if it happens in all the case
						}
						else
						{
							ft_dprintf(2, "[ZOMBIES CATCHER][PROCESS: \'%p\' STOP]\n", g_session->groups->active_processes);
							g_session->groups->active_processes->flags |= STOPPED;
						}
						// FREE HERE IS A VERY BAD IDEA (HIS SEMS ASSYCRONOUS TO MINISHELL) 
						/*
						if (!is_active_group(g_session->groups))
						{
							ft_dprintf(2, "NEXT: %p ---- PREV: %p\n", g_session->groups->next, g_session->groups->prev);
							g_session->groups->prev->next = g_session->groups->next;
							g_session->groups->next->prev = g_session->groups->prev;
							ft_dprintf(2, "[ZOMBIES CATCHER: %p FREED]\n", g_session->groups);
							free(g_session->groups);
							g_session->groups = NULL;
						}
						*/
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
	ft_dprintf(2, "minish> "); // JUST FOR TEST
}