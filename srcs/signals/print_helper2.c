/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_helper2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 04:49:24 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 22:01:53 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <signals.h>
#include <sys/wait.h>
#include <signal.h>

void			padding_spaces(int fd, size_t alreaddy_written)
{
	int			i;

	i = -1;
	while (++i < SIGNAL_PADDING - (int)alreaddy_written)
		write(fd, " ", 1);
}

void			print_group_line(int fd, t_group *group)
{
	int			i;

	i = -1;
	while (group && group->input && group->input[++i])
		ft_dprintf(fd, "%s%s", group->input[i], group->input[i + 1] ? " " : "");
}

char			*ft_norme_makes_my_code_worst(bool condition, char *ret_true,
		char *ret_false)
{
	if (condition)
		return (ret_true);
	return (ret_false);
}

void			print_index(int fd, t_process *target, int mode, int exit_st)
{
	const bool	is__leader = is_leader(target);
	t_group		*aux;
	char		*freed;

	freed = NULL;
	aux = group_get(target);
	ft_dprintf(fd, "%s%s%s", \
		ft_norme_makes_my_code_worst((stopped_signal_group(aux, true) \
	|| exit_st >= 0) && (!mode || (mode && is__leader)), "[", mode ? " " : ""),\
		ft_norme_makes_my_code_worst((stopped_signal_group(aux, true) \
	|| exit_st >= 0) && (!mode || (mode && is__leader)), \
freed = ft_itoa(background_index_get(g_session.nil, target)), mode ? " " : ""),\
		ft_norme_makes_my_code_worst((stopped_signal_group(aux, true) \
	|| exit_st >= 0) && (!mode || (mode && is__leader)), "]", mode ? " " : ""));
	free(freed);
}

void			print_history(int fd, t_process *target, int mode, int exit_st)
{
	const bool	is__leader = is_leader(target);
	t_group		*aux;

	aux = group_get(target);
	ft_dprintf(fd, "%s",
		ft_norme_makes_my_code_worst((stopped_signal_group(aux, true) \
			|| exit_st >= 0) && (!mode || (mode && is__leader)), \
			(char*)is_in_history(target), mode ? " " : ""));
}
