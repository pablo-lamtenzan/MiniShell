NAME	=		minishell
LIBFT	=		libft
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm

include srcs.mk

CFLAGS	=		-Wall -Wextra -Werror -g3 -fsanitize=address
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
OBJS_BONUS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS_BONUS))


all:			libft $(NAME)

libft:
	make -C $(LIBFT) NAME=libft.a CC="$(CC)" CFLAGS="$(CFLAGS)"

$(LIBFT)/libft.a: libft

$(NAME):		$(OBJS) $(LIBFT)/libft.a
	@echo LINK $@
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $@

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o:	$(SRCDIR)/%.c $(HDRS) Makefile | $(OBJDIR)
	@mkdir -p '$(@D)'
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

bonus:			LFLAGS += -lcurses -ltermcap
bonus:			BONUS = _bonus
bonus:			$(OBJS_BONUS) $(LIBFT)/libft.a
	@echo LINK $(NAME)
	$(CC) $(OBJS_BONUS) $(CFLAGS) $(LFLAGS) -o $(NAME)


.PHONY:			libft clean fclean prompt bonus

$(VERBOSE).SILENT:
