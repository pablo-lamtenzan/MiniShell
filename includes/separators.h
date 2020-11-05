/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separators.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 23:46:13 by pablo             #+#    #+#             */
/*   Updated: 2020/11/05 03:18:04 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term/lexer.h>

#ifndef SEPARATORS_H
# define SEPARATORS_H

# define OPEN_PAR       SCOPE_IN
# define CLOSE_PAR      SCOPE_OUT
# define AND            OP_AND
# define OR             OP_OR
# define SEMICOLON      SEP_SEMICOL
# define SKIP           4096
# define SKIPED_AND     8192
# define NOT_SKIPED_OR  16384

#endif