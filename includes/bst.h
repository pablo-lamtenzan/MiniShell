/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:45:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/03 21:34:01 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BST_H
# define BST_H

# include <stdlib.h>
# include <stdbool.h>

# define TOK_NONE       0
# define OP_REDIR_RD    1
# define OP_REDIR_WR    2
# define OP_REDIR_WA    4
# define OP_PIPE        8
# define TOK_CMD        16

# define NONE       TOK_NONE
# define REDIR_GR   OP_REDIR_WR
# define REDIR_LE   OP_REDIR_WA
# define REDIR_DG   OP_REDIR_RD
# define PIPE       OP_PIPE
# define CMD		TOK_CMD
# define FILENAME	32

typedef char        t_tok_t;

typedef struct      s_bst
{
	void*           a;
	void*           b;
	t_tok_t         type;
}                   t_bst;

typedef struct      s_tok
{
	t_tok_t         type;
	void*           data;
	struct s_tok*   next;
}                   t_tok;

t_tok*		        find_next_operator(t_tok* start, t_tok_t type);
t_tok*		        find_last_operator(t_tok* start, t_tok* end);
t_bst*		        new_node(void* a, void* b, t_tok_t type);
t_tok*		        is_pipe_cmd(t_tok* start);

t_bst*		        bst(t_tok* tokens);
void		        free_bst(t_bst* root);

#endif