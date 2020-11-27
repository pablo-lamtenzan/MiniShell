NAME	=		minishell
LIBFT	=		libft
SRCDIR	=		srcs
INCDIR	=		includes
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm
CFLAGS	=		-Wall -Wextra -Werror -g3 -fsanitize=address
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft -lcurses -ltermcap 

SRCS	=		$(addprefix $(SRCDIR)/,\
					$(addprefix term/,												\
						$(addprefix env/env,										\
							_get.c _set.c .c)										\
						$(addprefix lexer/,											\
							$(addprefix lexers/lex_,								\
								cmd.c ifs.c inline.c param_quoted.c param.c			\
								subshell.c tokens.c)								\
								lexer.c token_utils.c token.c)						\
						clip.c controls.c hist_cursor.c hist.c init.c				\
						cursor.c line_edit.c line_put.c line.c read_special.c		\
						read.c select.c												\
						term.c write.c)												\
					$(addprefix builtins/,\
						cd.c\
						echo.c\
						env.c\
						exit.c\
						export.c\
						pwd.c\
						unset.c\
						fg.c\
						bg.c\
						jobs.c\
						kill.c\
						wait.c\
						disown.c\
						builtins_utils.c\
					)\
					$(addprefix bst/,\
						bst_fill.c\
						bst.c\
					)\
					$(addprefix execution/,\
						execution_fd.c\
						execution_fill.c\
						redirection.c\
						execution.c\
					)\
					$(addprefix expansion/,\
						expand_param.c\
						expansion.c\
						word_split.c\
					)\
					$(addprefix separators/,\
						conditionals.c\
						separators.c\
						split_separators.c\
					)\
					$(addprefix signals/,\
						print_signals.c\
						print_helper1.c\
						print_helper2.c\
						print_helper3.c\
					)\
					$(addprefix job_control/,\
						process_lib.c\
						resume_suspended_processes.c\
						jobspec_parser.c\
						process.c\
						job_control.c\
						session.c\
						group.c\
						process_____.c\
						zombies_catcher.c\
					)\
					$(addprefix main/,\
						main.c\
					)\
					$(addprefix path/,\
						path.c\
					)\
				)

OBJDS	=		$(addprefix $(OBJDIR)/,\
					builtins\
					bst\
					execution\
					expansion\
					main\
					path\
					separators\
					signals\
					job_control\
					term\
					term/env\
					term/lexer\
					term/lexer/lexers\
				)

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

HDRS	=		$(addprefix $(INCDIR)/,\
					bst.h\
					builtins.h\
					execution.h\
					separators.h\
					path.h\
					signals.h\
					job_control.h\
					errors.h\
					$(addprefix term/, caps.h cursor.h env.h hist.h lexer.h\
					line.h term.h token.h)\
				)

all:			libft $(NAME)

libft:
	make -C $(LIBFT) NAME=libft.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(LIBFT)/libft.a: libft

$(NAME):		$(OBJDS) $(OBJS) $(LIBFT)/libft.a
	@echo LINK $(NAME)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(NAME)

prompt:
	clang -g3 -Wall -Wextra srcs/new_term/*.c srcs/env/*.c -I includes -Ilibft/includes -Llibft/ -lft -lcurses -o $@ -fsanitize=address -fsanitize=undefined 

$(OBJDS):
	mkdir -p $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.c $(HDRS) Makefile
	@echo CC $<
	@$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

clean:
	make -C $(LIBFT) $@
	@echo RM $(OBJDIR)
	@$(RM) -rf $(OBJDIR)

fclean:			clean
	make -C $(LIBFT) $@
	@echo RM $(NAME)
	@$(RM) -f $(NAME)

re:				fclean all

.PHONY:			libft clean fclean prompt

$(VERBOSE).SILENT:
