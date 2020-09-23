#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdbool.h>

#define PATH_MAX	4096

typedef struct		s_data
{
	int				ac;
	char			**argv;
	char			**env; // has to be a t_map
	int				pid;
	int				return_status;
}					t_data;

typedef struct		s_token
{
	char			*data;
	unsigned char	type;
	struct s_token	*next;
}					t_token;

typedef struct  	s_pipe
{
	int				fd[2];
	int				fd_read;
	int				fd_write;
	int				in;
	int				out;
}					t_pipe;

typedef struct		s_pipe2
{
	int				in[2];
	int				fd[2];
}					t_pipe2;

bool				free_five_ptrs(void **ptr1, void **ptr2, void **ptr3, void **ptr4
		, void **ptr5);
bool				free_four_ptrs_and_bst(void **ptr1, void **ptr2, void **ptr3, void **ptr4,
		t_bst **bst);
t_token				*token_clear(t_token **tokens);

#endif