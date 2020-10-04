/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:11:04 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/02 19:38:06 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <bst.h>

bool		free_five_ptrs(void *ptr1, void *ptr2, void *ptr3, void *ptr4,
		void **ptr5)
{
	free(ptr1);
	free(ptr2);
	free(ptr3);
	free(ptr4);
	free(ptr5);
	return (false);
}

bool		free_bst_node(t_bst **bst)
{
	if (bst && *bst)
	{
		free((void*)(*bst)->av[0]);
		free((void*)(*bst)->av[1]);
		free(*bst);
		*bst = NULL;
	}
	return (false);
}

bool		free_ptrs_and_bst(void *ptr1, void *ptr2, t_bst **bst)
{
	free(ptr1);
	free(ptr2);
	if (bst && *bst)
	{
		free((void*)(*bst)->av[0]);
		free((void*)(*bst)->av[1]);
		free(*bst);
		*bst = NULL;
	}
	return (false);
}
