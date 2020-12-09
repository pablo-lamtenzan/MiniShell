/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:39:43 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:52:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOBSPEC_PARSER_H
# define JOBSPEC_PARSER_H

/*
** Local
*/
# include <job_control/group.h>

t_process				**jobspec_parser(int ac, char*const *av,
						bool (*fill)(int ac, char*const *av));
t_process				**get_process_by_name(t_group *groups, const char *av);
bool					is_not_ambigous(t_process *target);
bool					is_not_ambigous_v2(const char *niddle);
bool					is_history_process(const char *string);
size_t					get_search_mode(const char *av);
size_t					get_history_index(const char *key);
bool					is_jobspec(const char *string);

#endif
