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
	char			type;
	struct s_token	*next;
}					t_token;

#endif