/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 03:28:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 11:17:49 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define CORE   "(core dumped)"

# include <term/process.h>

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

void	print_signals(int wsignal_index, const char** args, t_group* group);

#endif
