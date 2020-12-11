/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:57:00 by pablo             #+#    #+#             */
/*   Updated: 2020/12/11 20:51:58 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

/*
** External
*/
# include <stdbool.h>

void					zombies_catcher(int signal);
void					init_signal_handler(void);
void					ignore_all_signals(void);
void					do_nothing(int signal);
void					suspend_process(int signal);

#endif
