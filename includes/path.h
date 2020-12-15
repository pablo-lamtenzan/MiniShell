/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 20:55:39 by chamada           #+#    #+#             */
/*   Updated: 2020/12/14 15:31:55 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATH_H
# define PATH_H

# include <limits.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <stdbool.h>

# include <libft.h>

# include <string.h>
# include <errno.h>

# define PATH_DELIM	'/'

char		*path_cat(char *dest, const char *a, const char *b);
const char	*delim_skip(const char *path);

bool		path_get(char **dest, const char *name, const char *path);

#endif
