/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:11:04 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/22 21:22:15 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

bool		free_five_ptrs(void **ptr1, void **ptr2, void **ptr3, void **ptr4
		, void **ptr5)
{
	if (*ptr1)
		free(*ptr1);
	if (*ptr2)
		free(*ptr2);
	if (*ptr3)
		free(*ptr3);
	if (*ptr4)
		free(*ptr4);
	if (*ptr5)
		free(*ptr5);
	return (false);
}