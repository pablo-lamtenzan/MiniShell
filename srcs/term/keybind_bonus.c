/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybind_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/04 12:16:46 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_action	keybind_get(const t_keybind *bindings, size_t n, char key)
{
	size_t	i;

	i = 0;
	while (i < n && key != bindings[i].key)
		i++;
	return ((i < n) ? bindings[i].action : NULL);
}
