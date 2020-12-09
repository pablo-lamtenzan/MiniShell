NAME		=		minishell
NAME_BONUS	=		minish
LIBFT		=		libft
OBJDIR		=		objs
CC			=		/usr/bin/clang
RM			=		/bin/rm

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

$(NAME_BONUS): LFLAGS += -lcurses -ltermcap
$(NAME_BONUS): $(OBJS_BONUS) $(LIBFT)/libft.a
	@echo LINK $@
	$(CC) $(OBJS_BONUS) $(CFLAGS) $(LFLAGS) -o $@

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

bonus:			$(NAME_BONUS)

.PHONY:			libft clean fclean bonus

$(VERBOSE).SILENT:
