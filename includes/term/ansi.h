/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ansi.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 00:03:30 by chamada           #+#    #+#             */
/*   Updated: 2020/12/06 08:35:25 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANSI_H
# define ANSI_H

# define ANSI_ESC		'\033'
# define ANSI_CSI		'['
# define ANSI_ALTESC	'1'
# define ANSI_SHIFT		'2'

# define ANSI_CNTRL		'`'

/*
**	Default color
*/
# define C_D			"\033[39m"
/*
**	Default style
*/
# define S_D			"\033[0m"
/*
**	Bold style
*/
# define S_B			"\033[1m"
/*
**	Green intensity
*/
# define C_GI			"\033[92m"
/*
**	Red bright intensity
*/
# define C_RI			"\033[91m"
/*
**	Yellow bright intensity
*/
# define C_YI			"\033[93m"

#endif
