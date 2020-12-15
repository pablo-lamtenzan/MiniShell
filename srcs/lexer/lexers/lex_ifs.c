/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_ifs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 13:20:59 by chamada           #+#    #+#             */
/*   Updated: 2020/12/10 22:51:48 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <lexer/lexer.h>

/*
** IFS
** Whitespaces
**
** ( ' ' | [\t .. \r] )*
*/

t_lex_err	lex_ifs(t_lex_st *st)
{
	while (ft_isspace(*st->input))
		st->input++;
	if (*st->input == '\0' || *st->input == '#'
	|| (st->subshell_level != 0 && *st->input == ')'))
		return (LEX_EEND);
	return (LEX_EOK);
}
