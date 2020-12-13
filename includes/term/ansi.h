/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ansi.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 00:03:30 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/06 08:35:25 by: chamada         ###   ########lyon.fr   */
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
**	Light green
*/
# define C_GL			"\033[92m"
/*
**	Blue
*/
# define C_B			"\033[34m"
/*
**	Light Blue
*/
# define C_BL			"\033[94m"
/*
**	Yellow
*/
# define C_Y			"\033[33m"
/*
** Red
*/
# define C_R			"\033[0;31m"
/*
** White
*/
# define C_W			"\033[0;37m"
/*
**	Default style
*/
# define S_D			"\033[0m"
/*
**	Bold style
*/
# define S_B			"\033[1m"
/*
** Bold purple intesity
*/
# define C_PI			"\033[1;95m"
/*
** Bold white intesity
*/
# define C_WI			"\033[1;97m"
/*
** Bold green intensity
*/
# define S_BGI			"\033[1;92m"
/*
** Bold red intensity
*/
# define C_BRI			"\033[1;91m"
/*
** Bold yellow intensity
*/
# define C_BYI			"\033[1;93m"

#endif
