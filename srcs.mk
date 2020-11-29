INCDIR	=		includes
SRCDIR	=		srcs

HDRS	=	$(addprefix $(INCDIR)/,\
	bst.h\
	builtins.h\
	execution.h\
	separators.h\
	path.h\
	signals.h\
	job_control.h\
	errors.h\
	$(addprefix term/,\
		caps.h\
		line.h\
		term.h\
	)\
)

SRCS	=	$(addprefix $(SRCDIR)/,\
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
			tokens.c)\
			lexer.c\
			token_utils.c\
			token.c\
		)\
	$(addprefix term/,\
		caps.c caps_utils.c\
		clip.c\
		controls.c\
		cursor.c cursor_jmp.c\
		hist.c hist_cursor.c\
		keybind.c\
		line.c line_edit.c line_put.c\
		read.c read_csi.c read_esc.c\
		select.c select_jmp.c\
		signals.c\
		term.c\
		write.c\
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
		bg.c\
		jobs_helper.c\
		jobs.c\
		kill_helper.c\
		kill.c\
		wait_helper.c\
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
		allocators.c\
		background.c\
		conditions.c\
		dynamic_group_destructors.c\
		dynamic_process_destructors.c\
		exit_helper.c\
		group_operations.c\
		group.c\
		history_session.c\
		jobspec_parse_name.c\
		jobspec_parser_utils.c\
		jobspec_parser.c\
		process_operations.c\
		process.c\
		session.c\
		static_destructors.c\
		wait_processes.c\
		zombies_catcher.c\
		zombies.c\
	)\
	$(addprefix main/,\
		main.c\
	)\
	$(addprefix path/,\
		path.c\
	)\
)