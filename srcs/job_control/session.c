/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:08:59 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:36:12 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

bool			session_empty(void)
{
	return (g_session.nil->next == g_session.nil \
		&& g_session.nil->prev == g_session.nil);
}
