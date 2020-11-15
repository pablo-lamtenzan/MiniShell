/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:32:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/15 15:25:54 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

#include <unistd.h>
#include <stdbool.h>

#define PROCESSES_MAX   4096
#define MANAGE          0
#define LAST			0
#define PENULTIMATE		1

/* for the momment usses only s_process */
typedef struct 			s_process
{
	pid_t				pid;
    unsigned char       wstatus;
	char*const*			data;
	struct s_process*	next;
	struct s_process*	prev;
}						t_process;


/* Not official yet */
typedef struct			s_group
{
	t_process*			active_processes; // backgroud processes, first is leader process
	struct s_group*		next;
	pid_t				parent; // getpid() of minishell (inv fct but is bonus ...)
	bool				is_orphan; // true if all active processes's parent has exited, if is orphan an some process STOP -> SEND HUP + CONT to all processes in group
}						t_group;

// this could contain term and send to prompt &systhem->term in main
// this could be global too
// read about sessions before implement!!!!!
// ttin ctrl: openning /dev/tty and tcgetsid(fd)
typedef struct			s_systhem
{
	t_group*			groups; // all background processes by group
	t_process			processes[PROCESSES_MAX + 1]; // exec processes
	t_process			history[2]; // 2 last processes that still in bacground
	t_process*			zombies; // in case of need
}						t_systhem;


t_exec_status        	wait_processes(t_term* term, t_exec_status st);
size_t					suspended_process_nb(t_process* suspended);
bool					is_suspended(int wstatus);
void            		remove_suspended_process(t_process** suspended);
void                	resume_suspended_processes(t_process** suspended);
void					ft_swap(int* a, int* b);
void					update_used_pids(int new, pid_t** used_pids);

#endif