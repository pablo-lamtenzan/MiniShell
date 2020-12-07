/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:48:29 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:33:48 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

int				disown_init_exeption(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "rah", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s%s", \
	g_session.name, ": usage: disown: [-h] [-ar] [jobspec ... | pid ...]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session.groups->next == g_session.nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "%s: jobs: %s: no such job\n",\
				g_session.name, args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

int				disown_jobspec(t_exec *args, int flags, int nb, int i)
{
	t_process	**target;

	while (++i < args->ac - nb - 1)
	{
		if (!(target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
		{
			ft_dprintf(STDERR_FILENO, "%s: disown: %s: no such job\n", \
				g_session.name, args->av[nb + i + 1]);
			return (STD_ERROR);
		}
		disown_group(*target, flags, g_session.groups);
	}
	return (42);
}

int				b_disown(t_exec *args)
{
	int			flags;
	int			i;
	int			exept;
	int			nb;

	flags = 0;
	if ((exept = disown_init_exeption(args, &flags, &nb)) != 42)
		return (exept);
	i = -1;
	if (args->ac > 1)
	{
		if ((flags < 0 || !(flags & 2)) \
			&& (exept = disown_jobspec(args, flags < 0 \
			? -flags : flags, nb, i)) != 42)
			return (exept);
		else
			disown_all_groups(flags < 0 ? 0 : flags);
	}
	else if (g_session.hist)
		disown_group(g_session.hist->group->nil->next, \
			flags < 0 ? -flags : flags, g_session.groups);
	return (SUCCESS);
}
