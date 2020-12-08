NAME	=		minishell
LIBFT	=		libft
OBJDIR	=		objs
CC		=		/usr/bin/clang
RM		=		/bin/rm

include srcs.mk

CFLAGS	=		-Wall -Wextra -Werror #-g3 -fsanitize=address
IFLAGS	=		-I$(INCDIR) -I$(LIBFT)/includes
LFLAGS	=		-L$(LIBFT) -lft

OBJS	=		$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
BONUS_OBJS	=	$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(BONUS_SRCS))


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

.SECONDEXPANSION:
bonus:			LFLAGS += -lcurses -ltermcap
bonus:			BONUS = _bonus
bonus:			$$(BONUS_OBJS) $(LIBFT)/libft.a
	@echo LINK $@
	$(CC) $(BONUS_OBJS) $(CFLAGS) $(LFLAGS) -o $@


.PHONY:			libft clean fclean prompt

$(VERBOSE).SILENT:
