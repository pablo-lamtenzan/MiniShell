/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 17:33:51 by chamada           #+#    #+#             */
/*   Updated: 2020/12/14 17:33:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <path.h>

char		*path_cat(char *dest, const char *a, const char *b)
{
	const size_t	len = ft_strlen(a) + ft_strlen(b) + 1;
	char			*c;

	if (len >= PATH_MAX)
		return (NULL);
	c = dest;
	while (*a)
		*c++ = *a++;
	*c++ = PATH_DELIM;
	while (*b)
		*c++ = *b++;
	*c = '\0';
	return (dest);
}

const char	*delim_skip(const char *path)
{
	while (*path == PATH_DELIM)
		path++;
	return (path);
}
