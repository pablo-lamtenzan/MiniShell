/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 03:28:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/22 04:47:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define CORE   "(core dumped)"

# include <process.h>

# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <libft.h>

typedef struct      s_signal_print
{
    bool            printed;
    const char*     endline;
    unsigned int    process_nb;
    bool            plus;
    const char*     message;
    const char*     message_aux;
    bool    		has_args;
}                   t_signal_print;

void	            print_signals(t_process* target, t_group* group);
void				print_signal_v2(t_session* session, t_process* target, int flags);

#endif
