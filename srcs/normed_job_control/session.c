/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:08:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 01:19:06 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

bool			session_empty()
{
	return (g_session->nil->next == g_session->nil \
		&& g_session->nil->prev == g_session->nil);
}
