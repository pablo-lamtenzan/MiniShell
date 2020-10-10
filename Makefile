NAME	=		minishell
LIBFT	=		libft
SRCDIR	=		srcs
INCDIR	=		includes
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm
CFLAGS	=		-Wall -Wextra -Werror -g3 #-fsanitize=address
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft -lcurses -ltermcap

SRCS	=		$(addprefix $(SRCDIR)/,\
					$(addprefix builtins/,\
						cd.c\
						echo.c\
						env.c\
						exit.c\
						export.c\
						pwd.c\
						unset.c\
					)\
					bst.c\
					cmd.c\
					execution.c\
					execution_fill.c\
					free.c\
					lexer.c\
					main.c\
					operator.c\
					path.c\
					token.c\
				)

OBJDS	=		$(addprefix $(OBJDIR)/, builtins)

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

HDRS	=		$(addprefix $(INCDIR)/,\
					bst.h\
					builtins.h\
					cmd.h\
					executable.h\
					execution.h\
					lexer.h\
					minishell.h\
					operator.h\
					path.h\
					token.h\
				)

all:			libft $(NAME)

libft:
	make -C $(LIBFT) NAME=libft.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(LIBFT)/libft.a: libft

$(NAME):		$(OBJDS) $(OBJS) $(LIBFT)/libft.a
	@echo LINK $(NAME)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(NAME)

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

.PHONY:			libft clean fclean

$(VERBOSE).SILENT:
