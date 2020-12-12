/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 19:20:29 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 21:59:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>
#include <signals_print.h>
#include <job_control/jc_builtins.h>
#include <job_control/jobspec_parser.h>
#include <job_control/utils.h>

static int			wait_init_exeption(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "nf", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s: wait: %s: inalid option\n%s\n", \
			g_session.name, args->av[1], "wait: usage: wait [-fn] [id ...]");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session.groups->next == g_session.nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			if (!is_string_digit(args->av[1]))
				ft_dprintf(STDERR_FILENO, \
			"%s: wait: `%s\': not a pid or valid job spec\n", \
				g_session.name, args->av[1]);
			else
				ft_dprintf(STDERR_FILENO, \
					"%s: wait: pid %s: is not a child of this shell\n", \
						g_session.name, args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

static int			wait_jobspec(t_exec *args, int nb, int i,
	t_process ***target)
{
	if (!(*target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
	{
		if (is_string_digit(args->av[nb + i + 1]))
			ft_dprintf(STDERR_FILENO, \
				"%s: wait: pid %s is not a child of this shell\n", \
					g_session.name, args->av[nb + i + 1]);
		else
			ft_dprintf(STDERR_FILENO, "%s: wait: %s: no such job\n", \
					g_session.name, args->av[nb + i + 1]);
		return (CMD_NOT_FOUND);
	}
	return (42);
}

int					b_wait(t_exec *args)
{
	int				flags;
	static int		last_return = SUCCESS;
	int				tmp;
	t_process		**target;
	int				nb;

	flags = 0;
	if ((tmp = wait_init_exeption(args, &flags, &nb)) != 42)
		return (tmp);
	flags = flags < 0 ? -flags : flags;
	if (args->ac > 1 && args->ac > nb + 1 && 0 < args->ac - nb - 1)
	{
		if ((tmp = wait_jobspec(args, nb, 0, &target)) != 42)
			return (tmp);
		tmp = last_return;
		last_return = wait_group(*target, flags, g_session.groups);
		return (!flags ? tmp : last_return);
	}
	wait_all_groups(flags);
	return (SUCCESS);
}
