/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 21:44:53 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/08 22:39:03 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

bool				dup_stdio(int fds[3])
{
	int32_t			i;

	i = -1;
	while (++i < 3)
		if (fds[i] != i \
				&& (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
	return (true);
}

bool			if_builtin_exec(const t_builtin_exec b, t_term* term, int32_t fds[3])
{
	const t_builtin builtin = builtin_get(b.av[0]);
	t_builtin_args	args;

	if (builtin)
	{
		args.ac = b.ac;
		args.av = b.av;
		args.t = term;
		if (!dup_stdio(fds))
			return (false);
		args.fds[0] = fds[1];
		args.fds[1] = fds[0];
		term->st = builtin(&args);
		close_fds(args.fds);
	}
	return (builtin);
}