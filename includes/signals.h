/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 03:28:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 03:47:01 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

// delete this includes after test
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct      s_signal_print
{
    bool            printed;
    const char*     endline;
    unsigned int    process_nb;
    const char*     plus;
    const char*     message;
    const char*     message_aux;
    bool    		has_args;
}                   t_signal_print;

void	print_signals(int wsignal_index, char *const* args);

#endif
