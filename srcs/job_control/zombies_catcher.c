/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 03:11:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 08:23:37 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <libft.h>

// do ./minishell, then ./a.out, then ctrl+D and its works

void			zombies_catcher(int signal)
{
	(void)signal;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[TEST] HAVE TO DISPLAY THIS MSG FOR EACH TERMINATED CHILD (yeah works nice!)\n");

	
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
				/* LIKE:
				
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
		*/
		
	}
}