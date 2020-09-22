#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct		s_data
{
	int				ac;
	char			**argv;
	char			**envp;
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

bool		free_five_ptrs(void *ptr1, void *ptr2, void *ptr3, void *ptr4
		, void *ptr5);

#endif