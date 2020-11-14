/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 11:35:08 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:47:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

/*
** Process return
*/
# define SUCCESS		0
# define STD_ERROR		1
# define CMD_BAD_USE	2
# define CMD_CANT_EXEC	126
# define CMD_NOT_FOUND  127
# define INV_EXIT_ARG	128
# define SIGNAL_BASE	INV_EXIT_ARG

/*
** Error redirection
*/
# define FILENOTFOUND	-1
# define AMB_REDIRECT	-2
# define FATAL_ERROR	-3

#endif