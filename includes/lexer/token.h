/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 03:56:49 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 03:56:49 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <stdint.h>
# include <stdlib.h>

# include <libft.h>

/*
** Token types
*/
# define TOK_NONE		0

# define TOK_SEP		1

# define TOK_SCOPE_IN	2
# define TOK_SCOPE_OUT	4

# define TOK_MSCOPE		6

# define TOK_PARAM		8
# define TOK_SIMPLE		16
# define TOK_SQUOTED	32
# define TOK_DQUOTED	64

# define TOK_MQUOTED	96

# define TOK_CMD		128

# define OP_OR			256
# define OP_AND			512
# define OP_PIPE		1024

# define OP_M			1792

# define OP_REDIR_WR	2048
# define OP_REDIR_WA	4096
# define OP_REDIR_RD	8192
# define OP_HEREDOC		16384

# define OP_MREDIR		30720

typedef uint16_t	t_tok_t;

typedef struct		s_tok
{
	t_tok_t			type;
	void			*data;
	struct s_tok	*next;
}					t_tok;

t_tok				*token_new(void *data, t_tok_t type);
t_tok				*token_strndup(const char *input, size_t n, t_tok_t type);

void				token_add_back(t_tok **tokens, t_tok *token);
void				token_add_front(t_tok **tokens, t_tok *token);

void				token_sort(t_tok **lst, int (*cmp)(t_tok_t, t_tok_t));

size_t				token_len(t_tok *tokens);

void				token_clr(t_tok **tokens);

#endif
