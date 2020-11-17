/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resume_suspended_processes.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 11:29:48 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 17:40:27 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signal.h>

// in use
bool			is_suspended(int wstatus)
{
	return (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)) \
			&& (wstatus == SIGSTOP || wstatus == SIGTSTP \
			|| wstatus == SIGTTIN || wstatus == SIGTTOU));
}
