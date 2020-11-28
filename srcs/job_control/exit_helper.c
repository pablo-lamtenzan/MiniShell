/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:30:21 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 03:42:15 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void		handle_exit_with_active_background(int exit_status)
{
	if (g_session->exit_count++ == 2 || !is_background_active())
		exit(exit_status);
	else
		write(STDERR_FILENO, "There are stopped jobs.\n", 25);
}

void		update_exit_count(const char* name)
{
	if (!ft_strncmp(name, "exit", 5) && g_session->exit_count == 1)
		g_session->exit_count++;
	else
		g_session->exit_count = 0;
}
