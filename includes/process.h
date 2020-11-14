/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 08:09:18 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

#include <unistd.h>

#define PROCESSES_MAX     4096
#define MANAGE          0

typedef struct 			s_process
{
	pid_t				pid;
    unsigned char       wstatus;
	char*const*			data;
}						t_process;

bool                    wait_processes(t_process* processes, int* term_st);

#endif