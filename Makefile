NAME	=		minishell
LIBFT	=		libft
SRCDIR	=		srcs
INCDIR	=		includes
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm
CFLAGS	=		-Wall -Wextra -Werror -g3
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft -lcurses -ltermcap

SRCS	=		$(addprefix $(SRCDIR)/,											\
					$(addprefix builtins/,										\
						cd.c echo.c env.c exit.c export.c pwd.c unset.c)		\
					bst.c cmd.c execution.c free.c lexer.c main.c operator.c	\
					path.c pipe.c token.c)

OBJDS	=		$(addprefix $(OBJDIR)/, builtins)

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

HDRS	=		$(addprefix $(INCDIR)/, bst.h builtins.h cmd.h lexer.h			\
					minishell.h operator.h path.h token.h)

all:			libft $(NAME)

libft:
    make -C $(LIBFT) libft.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(LIBFT)/libft.a: libft

$(OBJDS):
	mkdir -p $@
	
$(NAME):		$(OBJDS) $(OBJS) $(LIBFT)/libft.a
	@echo LINK $(NAME)
	$(CC) $(OBJS) $(CFLAGS) $(IFLAGS) $(LFLAGS) -o $(NAME)


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

.PHONY	=		all libft clean fclean re

$(VERBOSE).SILENT:
