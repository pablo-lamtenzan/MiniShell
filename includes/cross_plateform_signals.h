/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross_plateform_signals.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 13:10:06 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 17:28:37 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>

/*
** Miscellaneous signals
** More info: https://en.wikipedia.org/wiki/Signal_(IPC)#Handling_signals
*/

// to do kill needs to handle that and print "nor avalaible or smthing"

#ifndef SIGEMT
# define SIGEMT -1
#endif

#ifndef SIGINFO
# define SIGINFO -1
#endif

#ifndef SIGPWR
# define SIGPWR -1
#endif

#ifndef SIGLOST
# define SIGLOST -1
#endif

#ifndef SIGSTKFLT
# define SIGSTKFLT -1
#endif

#ifndef SIGUNUSED
# define SIGUNUSED -1
#endif

#ifndef SIGCLD
# define SIGCLD -1
#endif
