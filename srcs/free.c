/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plamtenz <plamtenz@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:11:04 by plamtenz          #+#    #+#             */
/*   Updated: 2020/09/23 17:39:53 by plamtenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <bst.h>

bool		free_five_ptrs(void **ptr1, void **ptr2, void **ptr3, void **ptr4,
		void **ptr5)
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

bool		free_four_ptrs_and_bst(void **ptr1, void **ptr2, void **ptr3, void **ptr4,
		t_bst **bst)
{
	if (*ptr1)
		free(*ptr1);
	if (*ptr2)
		free(*ptr2);
	if (*ptr3)
		free(*ptr3);
	if (*ptr4)
		free(*ptr4);
	if (*bst)
	{
		if ((*bst)->cmd[0])
			free((*bst)->cmd[0]);
		if ((*bst)->cmd[1])
			free((*bst)->cmd[1]);
		free(*bst);
	}
	return (false);
}