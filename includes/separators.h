/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 23:46:13 by pablo             #+#    #+#             */
/*   Updated: 2020/11/11 21:53:33 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term/token.h>

#ifndef SEPARATORS_H
# define SEPARATORS_H

# define OPEN_PAR       TOK_SCOPE_IN
# define CLOSE_PAR      TOK_SCOPE_OUT
# define AND            OP_AND
# define OR             OP_OR
# define SEMICOLON      TOK_SEP
# define SKIP           4096
# define SKIPED_AND     8192
# define NOT_SKIPED_OR  16384

#endif
