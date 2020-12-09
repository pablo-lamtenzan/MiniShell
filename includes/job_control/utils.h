/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 22:51:31 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:53:07 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/*
** Local
*/
# include <job_control/process.h>

# define ESPACE			' '

bool					is_string_digit(const char *string);
int						matrix_height(char **matrix);
bool					ignore_pid(int ac, char*const *av);
int						parse_flags(int ac, char*const *av, const char *pattern,
						int *nb_flags);
const char				*is_in_history(t_process *target);
char					**split_separators(char *input);
int						for_each_in_group(t_process *leader,
						int (*core)(), bool(*delete)());
#endif
