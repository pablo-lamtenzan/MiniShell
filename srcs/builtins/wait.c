/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:20:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/18 20:41:03 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

void		f_flag(t_session* s_session, t_process** target)
{
	// TO DO
}

int			parse_flags(int ac, char** av)
{
	size_t i;
	int flags;

	flags = 0;
	i = 0;
	while (++i < ac)
	{
		if (!ft_strncmp(av[i], "-n", 2))
			flags |= 1;
		else if (!ft_strncmp(av[i], "-f", 2))
			flags |= 2;
		else
			return (-1);
	}
	return (flags);
}

int			ft_wait(t_exec* exec, t_term* term)
{
	/*
	wait [-fn] [jobspec or pid]:
 -> Wait until the child process (jobspec or pid) exits
 -> Returns the exit status of the last command waited for
 -> If a jobspec is given, all the processes in the job are waited for
 -> No options, all currently active childs are waited, returns 0
 -> '-n' wait for a single job and return its exit status
 -> '-f' forces to wait for each job to terminate before returning its status intead of returning when it changes status
 -> Job not found return 127
 */
	int flags;
	static int last_return = SUCCESS;
	t_process**	target;
	t_group*	groups;

	if (exec->ac > 1)
	{
		if ((flags = parse_flags(exec->ac, exec->av) < 0)) // lexer error flags
		{
			ft_dprintf(2, "bash: wait: `%s\': not a pid or valid job spec\n", exec->av[1]);
			return (CMD_NOT_FOUND);
		}
		// take target process
		if ((target = jobspec_parser(term->session, exec->ac, flags > 0 ? exec->av : &exec->av[1], NULL)))
		{
			if (flags & 1 || !flags) // -n or no flags always before f (for return in case there are both)
			{
				if (WIFSTOPPED((*target)->wstatus))
				{
					ft_dprintf(2, "bash: warning: wait_for_job: job %d is stopped\n", ft_atoi(&exec->av[1][1]));
					return (148);
				}
				update_background(term->session, target);
				return (flags ? WEXITSTATUS((*target)->wstatus) : last_return); // problem here i need the return of the target but target will be freed in update_background
			}
			else if (flags & 2) // -f
			{
				// TO DO
			}			
		}
		else if (!flags)
		{
			// invalid jobspec (not found)
			ft_dprintf(2, "bash: wait: %s: no such job\n", exec->av[0]);
			return (CMD_NOT_FOUND);
		}
		else if (flags & 1) // -n always before f (for return in case there are both)
			return (CMD_NOT_FOUND);
		else if (flags & 2) // -f
		{
			// TO DO
		}
	}
	else
	{
		// no args
		// wait for all
		groups = term->session->groups;
		while (groups != term->session->nil)
		{
			while (groups->active_processes != groups->nil)
			{
				// check if the process is running (not stopeed)and wait for it
				if (!WIFSTOPPED(groups->active_processes->wstatus) && !WIFEXITED(groups->active_processes->wstatus)) // same bad arg
				{
					update_background(term->session, &groups->active_processes); // this doesn't work, have to send the addr in session
					return (last_return);
				}
				else if (WIFSTOPPED(groups->active_processes->wstatus)) // if istopped warning
				{
					ft_dprintf(STDERR_FILENO, "bash: wait: warning: job %d[%d] stopped", get_background_index(groups, groups->active_processes), 1/*background_find(groups->active_processes, "PID", groups)*/); // this arguments sre bad, need reference the session
					return (last_return);
				}
				else if (WIFEXITED(groups->active_processes->wstatus)) // if process has end update return value and continue
				{
					last_return = WEXITSTATUS(groups->active_processes->wstatus);
					groups->active_processes = groups->active_processes->next;
				}
			}
			groups = groups->next;
		}
	}
	return (last_return);
}