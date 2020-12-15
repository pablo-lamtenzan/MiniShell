/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 22:41:13 by chamada           #+#    #+#             */
/*   Updated: 2020/12/14 22:41:26 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <term/term.h>
# include <lexer/lexer.h>
# include <execution/execution.h>
# include <separators.h>
# include <builtins.h>
# include <signals_print.h>
# include <job_control/utils.h>
# include <signal_handler.h>

# include <string.h>
# include <unistd.h>

void	handle_exec_error(t_bst *root, t_exec_status exec_st);
void	syntax_error(t_lex_st *st);

#endif
