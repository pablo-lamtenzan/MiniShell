/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:45:17 by pablo             #+#    #+#             */
/*   Updated: 2020/12/11 23:02:45 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BST_H
# define BST_H

/*
** External
*/
# include <stdlib.h>
# include <stdbool.h>

/*
** Local
*/
# include <lexer/token.h>

/*
** Operators
*/
# define NONE		TOK_NONE
# define REDIR_GR	OP_REDIR_WR
# define REDIR_LE	OP_REDIR_RD
# define REDIR_DG	OP_REDIR_WA
# define REDIR_DL	OP_HEREDOC
# define PIPE		OP_PIPE
# define CMD		TOK_CMD
# define REDIRECT	OP_MREDIR

typedef struct		s_bst
{
	void			*a;
	void			*b;
	t_tok_t			type;
}					t_bst;

/*
** BST create
*/
t_bst				*bst(t_tok *tokens);

/*
** BST destroy
*/
void				free_bst(t_bst *root);

/*
** BST fill
*/
t_tok				*find_next_operator(t_tok *start, t_tok_t type);
t_tok				*find_last_operator(t_tok *start, t_tok *end);
t_bst				*new_node(void *a, void *b, t_tok_t type);
t_tok				*is_pipe_cmd(t_tok *start);

#endif
