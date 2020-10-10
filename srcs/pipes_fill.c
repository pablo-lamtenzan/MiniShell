/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/08 21:44:53 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/10 15:48:20 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
/* 
bool				dup_stdio(int fds[3])
{
	int32_t			i;

	i = -1;
	while (++i < 3)
		if (fds[i] != i && (dup2(fds[i], i) < 0 || close(fds[i]) < 0))
			return (false);
	return (true);
}
 */
