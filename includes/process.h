/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 10:28:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

#include <unistd.h>
#include <stdbool.h>

#define PROCESSES_MAX   4096
#define MANAGE          0

typedef struct 			s_process
{
	pid_t				pid;
    unsigned char       wstatus;
	void*				data;
}						t_process;

bool        			wait_processes(t_term* term);
size_t					suspended_process_nb(t_process* suspended);;

#endif