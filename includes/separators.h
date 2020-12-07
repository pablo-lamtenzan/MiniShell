/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 23:46:13 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:37:40 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEPARATORS_H
# define SEPARATORS_H

/*
** Local
*/
# include <lexer/token.h>

/*
** Separators
*/
# define OPEN_PAR       TOK_SCOPE_IN
# define CLOSE_PAR      TOK_SCOPE_OUT
# define AND            OP_AND
# define OR             OP_OR
# define SEMICOLON      TOK_SEP
# define SKIP           4096
# define SKIPED_AND     8192
# define NOT_SKIPED_OR  16384

/*
** Indexes
*/
# define CONDITIONALS   0
# define STATUS         1
# define PARETHESES_NB  2

#endif
