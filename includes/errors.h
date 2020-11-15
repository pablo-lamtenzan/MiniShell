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

typedef enum			e_exec_status
{
	SUCCESS				= 0,
	STD_ERROR			= 1,
	CMD_BAD_USE			= 2,
	CMD_CANT_EXEC		= 126,
	CMD_NOT_FOUND		= 127,
	INV_EXIT_ARG		= 128,
	SIGNAL_BASE			= 128,
	BAD_ALLOC			= SIGNAL_BASE + SIGABRT,
	BAD_CLOSE			= 222, // to define
	BAD_PIPE			= 222, // to define
	BAD_DUP2			= 222, // to define
	BAD_FORK			= 222 // to define
}						t_exec_status;

typedef enum			e_redir_status
{
	CONTINUE			= 0,
	FILE_NOT_FOUND		= -1,
	AMDIG_REDIRECT		= -2,
	FLNAME_TO_LONG		= -3,
	BAD_ALLOC			= -4
}						t_redir_status;

#endif