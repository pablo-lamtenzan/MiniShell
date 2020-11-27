/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 03:11:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 00:47:11 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <libft.h>

// do ./minishell, then ./a.out, then ctrl+D and its works

/*
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
							g_session->groups->active_processes->flags |= (EXITED | NO_DELETE); // print too
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
*/

// New zombie catcher: uses a new data structure who has the groups addresses
// The idea is to not iterate over the groups cause this turns in async
// Iterate async over the groups can cause "heap use after free" if theres a SIGCHLD when minishell is deleting a group
// NOW: only iterates over non deletable groups, so might work nice

/*
void		zombie_catcher_v2(int signal)
{
	(void)signal;
	t_background*	remember;
	t_background*	next;
	bool			skip;
	bool			first_freed;
	t_process*		remember_leader;
	int				wreturn;

	if (!g_session || g_session->restrict_zombies)
		return ;

	ft_dprintf(2, "[ZOMBIE CATCHER][FIRST ZOMBIE IS: %p]\n", g_session->zombies);
	if (g_session->zombies)
	{
		ft_dprintf(2, "[ZOMBIE CATCHER][FIRST ZOMBIE NEXT IS: %p]\n", g_session->zombies->next);
		if (*g_session->zombies->background_group)
			ft_dprintf(2, "[Z C][FIRST BCKGROUND FLAGS: %d][process: %p]\n", (*g_session->zombies->background_group)->active_processes->flags, (*g_session->zombies->background_group)->active_processes);
	}
	t_background* prev = NULL;

	remember = g_session->zombies;
	first_freed = false;
	while (g_session->zombies)
	{
		skip = false;
		next = g_session->zombies->next;
		if (*g_session->zombies->background_group)
		{
			remember_leader = (*g_session->zombies->background_group)->active_processes;
			while ((*g_session->zombies->background_group)->active_processes != (*g_session->zombies->background_group)->nil)
			{
				if ((*g_session->zombies->background_group)->active_processes->flags & BACKGROUND)
				{
					wreturn = waitpid((*g_session->zombies->background_group)->active_processes->pid, &(*g_session->zombies->background_group)->active_processes->wstatus, WNOHANG | WUNTRACED);
					if (wreturn == -1)
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2][WAITPID ERROR]\n");
					}
					else if (wreturn == 0)
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2][PROCESS \'%p\' IS IN BACKGROUND]\n", (*g_session->zombies->background_group)->active_processes);
					}
					else
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2 CATCHED \'%p\' SUCESFULLY!]\n", (*g_session->zombies->background_group)->active_processes);
						
						// This makes the node in g_session->groups will be rm later (now causse asyncrous results, not preditable)
						(*g_session->zombies->background_group)->active_processes->flags &= ~BACKGROUND;

						if (WIFEXITED((*g_session->zombies->background_group)->active_processes->wstatus))
						{
							// This is for print "Done" in jobs
							(*g_session->zombies->background_group)->active_processes->flags |= EXITED;

							// This is for the return status
							(*g_session->zombies->background_group)->active_processes->ret = WEXITSTATUS((*g_session->zombies->background_group)->active_processes->wstatus);
							endzombie_push_back(endzombie_new(&(*g_session->zombies->background_group)->active_processes));
		
						}
						else if (WIFSTOPPED((*g_session->zombies->background_group)->active_processes->wstatus))
						{
							g_session->st = SIGNAL_BASE + WSTOPSIG((*g_session->zombies->background_group)->active_processes->wstatus);
							(*g_session->zombies->background_group)->active_processes->flags |= STOPPED;
						}
						else
						{
							g_session->st = SIGNAL_BASE + WTERMSIG((*g_session->zombies->background_group)->active_processes->wstatus);
						}
						if (!((*g_session->zombies->background_group)->active_processes->flags & BACKGROUND))
						{
							skip = true;
							if (remember == g_session->zombies)
								first_freed = true;
		
							(*g_session->zombies->background_group)->active_processes = remember_leader;
							g_session->zombies->exited = true;
							ft_dprintf(2, "[ZOMBIE CATCHER][ZOMBIE %p CONTANING %p HAS EXITED!]\n", g_session->zombies, *g_session->zombies->background_group);
							// IF THIS CAUSES PROBLEMS I CAN JUST MARK THE NODES AND FREE THEM NEXT

							
							//remove_zombie_node(*g_session->zombies->background_group);
							//t_background* x = g_session->zombies->next;
							//ft_dprintf(2, "[ZOMBIE CATHER][RM %p][CONTAINING: %p]\n", g_session->zombies, *g_session->zombies->background_group);
							//free(g_session->zombies);
							//g_session->zombies = x;
							//ft_dprintf(2, "[ZOMBIE CACTHER][ZOMBIES ARE NOW: %p][FIRST FREED IS: %d]\n", g_session->zombies, first_freed);
							//if (x && prev)
							//	prev->next = g_session->zombies;

							
							//break ;
						}
					}
				}
				(*g_session->zombies->background_group)->active_processes = (*g_session->zombies->background_group)->active_processes->next;
			}
			//if (!skip)
			(*g_session->zombies->background_group)->active_processes = remember_leader;
		}
		prev = g_session->zombies;
		g_session->zombies = next;
	}
	//g_session->zombies = first_freed ? NULL : remember;
	g_session->zombies = remember;
	ft_dprintf(2, "[ZOMBIES CATCHER AT THE END: %p]\n", g_session->zombies);
}
*/


void		zombie_catcher_v2(int signal)
{
	(void)signal;
	t_background*	remember;
	t_background*	next;
	bool			skip;
	bool			first_freed;
	t_process*		remember_leader;
	int				wreturn;

	if (!g_session || g_session->restrict_zombies)
		return ;

	ft_dprintf(2, "[ZOMBIE CATCHER][FIRST ZOMBIE IS: %p]\n", g_session->zombies);
	if (g_session->zombies)
	{
		ft_dprintf(2, "[ZOMBIE CATCHER][FIRST ZOMBIE NEXT IS: %p]\n", g_session->zombies->next);
		if (g_session->zombies->background_group)
			ft_dprintf(2, "[Z C][FIRST BCKGROUND FLAGS: %d][process: %p]\n", (g_session->zombies->background_group)->active_processes->flags, (g_session->zombies->background_group)->active_processes);
	}
	t_background* prev = NULL;

	remember = g_session->zombies;
	first_freed = false;
	while (g_session->zombies)
	{
		skip = false;
		next = g_session->zombies->next;
		if (g_session->zombies->background_group)
		{
			remember_leader = (g_session->zombies->background_group)->active_processes;
			while ((g_session->zombies->background_group)->active_processes != (g_session->zombies->background_group)->nil)
			{
				if ((g_session->zombies->background_group)->active_processes->flags & BACKGROUND)
				{
					wreturn = waitpid((g_session->zombies->background_group)->active_processes->pid, &(g_session->zombies->background_group)->active_processes->wstatus, WNOHANG | WUNTRACED);
					if (wreturn == -1)
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2][WAITPID ERROR]\n");
					}
					else if (wreturn == 0)
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2][PROCESS \'%p\' IS IN BACKGROUND]\n", (g_session->zombies->background_group)->active_processes);
					}
					else
					{
						//if (PRINT_DEBUG)
							ft_dprintf(2, "[ZOMBIES CATCHER V2 CATCHED \'%p\' SUCESFULLY!]\n", (g_session->zombies->background_group)->active_processes);
						
						// This makes the node in g_session->groups will be rm later (now causse asyncrous results, not preditable)
						(g_session->zombies->background_group)->active_processes->flags &= ~BACKGROUND;

						if (WIFEXITED((g_session->zombies->background_group)->active_processes->wstatus))
						{
							// This is for print "Done" in jobs
							(g_session->zombies->background_group)->active_processes->flags |= EXITED;

							// This is for the return status
							(g_session->zombies->background_group)->active_processes->ret = WEXITSTATUS((g_session->zombies->background_group)->active_processes->wstatus);
							endzombie_push_back(endzombie_new(&(g_session->zombies->background_group)->active_processes));
		
						}
						else if (WIFSTOPPED((g_session->zombies->background_group)->active_processes->wstatus))
						{
							g_session->st = SIGNAL_BASE + WSTOPSIG((g_session->zombies->background_group)->active_processes->wstatus);
							(g_session->zombies->background_group)->active_processes->flags |= STOPPED;
						}
						else
						{
							g_session->st = SIGNAL_BASE + WTERMSIG((g_session->zombies->background_group)->active_processes->wstatus);
						}
						if (!((g_session->zombies->background_group)->active_processes->flags & BACKGROUND))
						{
							skip = true;
							if (remember == g_session->zombies)
								first_freed = true;
		
							(g_session->zombies->background_group)->active_processes = remember_leader;
							g_session->zombies->exited = true;
							ft_dprintf(2, "[ZOMBIE CATCHER][ZOMBIE %p CONTANING %p HAS EXITED!]\n", g_session->zombies, g_session->zombies->background_group);
							// IF THIS CAUSES PROBLEMS I CAN JUST MARK THE NODES AND FREE THEM NEXT

							/*
							//remove_zombie_node(*g_session->zombies->background_group);
							t_background* x = g_session->zombies->next;
							ft_dprintf(2, "[ZOMBIE CATHER][RM %p][CONTAINING: %p]\n", g_session->zombies, *g_session->zombies->background_group);
							free(g_session->zombies);
							g_session->zombies = x;
							ft_dprintf(2, "[ZOMBIE CACTHER][ZOMBIES ARE NOW: %p][FIRST FREED IS: %d]\n", g_session->zombies, first_freed);
							if (x && prev)
								prev->next = g_session->zombies;

							*/
							//break ;
						}
					}
				}
				(g_session->zombies->background_group)->active_processes = (g_session->zombies->background_group)->active_processes->next;
			}
			//if (!skip)
			(g_session->zombies->background_group)->active_processes = remember_leader;
		}
		prev = g_session->zombies;
		g_session->zombies = next;
	}
	//g_session->zombies = first_freed ? NULL : remember;
	g_session->zombies = remember;
	ft_dprintf(2, "[ZOMBIES CATCHER AT THE END: %p]\n", g_session->zombies);
}