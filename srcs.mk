INCDIR		=	includes
SRCDIR		=	srcs

HDRS		=	$(addprefix $(INCDIR)/,\
	bst.h\
	builtins.h\
	execution.h\
	separators.h\
	path.h\
	signals_print.h\
	cross_plateform_signals.h\
	errors.h\
	$(addprefix term/,\
		ansi.h\
		caps.h\
		hist.h\
		line.h\
		term.h\
	)\
	$(addprefix job_control/,\
		background.h\
		conditions.h\
		destructors.h\
		group.h\
		history.h\
		jc_builtins.h\
		jobspec_parser.h\
		process.h\
		session.h\
		utils.h\
		zombies.h\
	)\
)

SRCS_COMMON =\
$(addprefix env/env,\
	_get.c\
	_set.c\
	.c\
)\
$(addprefix lexer/,\
	$(addprefix lexers/lex_,\
		cmd.c\
		ifs.c\
		inline.c\
		param_quoted.c\
		param.c\
		subshell.c\
		tokens.c\
	)\
	lexer.c\
	token_utils.c\
	token.c\
	)\
$(addprefix term/,\
	line.c line_edit.c line_put.c\
	read.c\
)\
$(addprefix builtins/,\
	cd.c\
	echo.c\
	env.c\
	exit.c\
	export.c\
	pwd.c\
	unset.c\
	fg.c\
	history.c\
	bg.c\
	jobs_helper.c\
	jobs.c\
	kill_helper.c\
	kill.c\
	wait_helper.c\
	wait.c\
	disown_helper.c\
	disown.c\
	builtins.c\
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
$(addprefix signals_print/,\
	print_signals.c\
	print_helper1.c\
	print_helper2.c\
	print_helper3.c\
)\
$(addprefix job_control/,\
	$(addprefix group/,\
		group_operations.c\
		group.c\
		return_st.c\
	)\
	$(addprefix jobspec_parser/,\
		jobspec_parser_name.c\
		jobspec_parser_utils.c\
		jobspec_parser_utils2.c\
		jobspec_parser.c\
	)\
	$(addprefix memory/,\
		allocators.c\
		dynamic_group_destructors.c\
		dynamic_process_destructors.c\
		static_destructors.c\
		static_destructors2.c\
	)\
	$(addprefix process/,\
		process_operations.c\
		process.c\
	)\
	$(addprefix zombies/,\
		dead_zombies.c\
		zombies_catcher.c\
		zombies.c\
	)\
	background.c\
	conditions.c\
	exit_helper.c\
	history_session.c\
	print_terminated.c\
	session.c\
	wait_processes.c\
)\
$(addprefix main/,\
	main.c\
	async.c\
)\
$(addprefix path/,\
	path.c\
)

SRCS		=	$(addprefix $(SRCDIR)/,\
	$(SRCS_COMMON)\
	$(addprefix term/,\
		term.c\
	)\
)

SRCS_BONUS	=	$(addprefix $(SRCDIR)/,\
	$(SRCS_COMMON)\
	$(addsuffix _bonus.c,\
		$(addprefix term/,\
			caps caps_goto caps_utils\
			clear\
			clip\
			control control_line\
			cursor cursor_jmp\
			hist hist_cursor\
			keybind\
			read_caps read_csi read_esc\
			select select_jmp\
			term\
			write\
		)\
	)\
)
