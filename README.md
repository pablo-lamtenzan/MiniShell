<p align="center">
	<h2 align="center">minishell</h2>
	<p align="center">An implementation of a Shell from scratch by ClemaX & pablo-lamtenzan</p>
</p>

---
# Table of Contents
0. [TODO](#todo)
1. [Usage](#usage)
	- TODO
2. [How it works](#how-it-works)
	- [Tokenization](#tokenization)
	- [Parsing](#parsing)
	- [Expansion](#expansion)
	- [Execution](#execution)
3. [Specifications](#specifications)
	- [Key Bindings](#key-bindings)
	- [Token Types](#token-types)
	- [Expansion Types](#expansion-types)
	- [Builtin Commands](#built-in-commands)
	- [Job Control](#job-control)

# TODO
- Readme
	- [ ] Document terminal capabilities
	- [ ] Document execution environment
	- [ ] Document shell expansion
	- [ ] Document job control

# Usage
### Dependencies
- GNU `make`
- a recent version of `clang`
- the `libtermcap` library, including it's development headers

### Compiling minishell
```sh
make
```
### Running minishell
```sh
./minishell
```

# How it works

## Tokenization

First of all, lets talk about the separators. The separators are:
```
";", "&&", "||"
```
A separator is always the end of the execution. So, that means if there's input command with n separators it can be splited n times in shorter commands.
Lets call this splits result a: "command line". A command line can be splited too, we can use the pipe as separator. Lets call this commands lines splits result a: "job". And finally, a job can be splited too, into simple commands.
```
INPUT CMD -> can be composed by n CMD LINE(s) (separeted by ";", "&&", "||")
CMD LINE -> can be composed by n JOB(s) (separeted by "|")
JOB -> contains allways a command and n redirections ("<", "<<", ">", ">>")
```
The cmd lines and jobs will be handled in the BST (see in next part). The lexer-parser will return a linked-list of tokens that will be used to build a BST later. 
This is the struct of a token node:
```
typedef struct      s_tok
{
	t_tok_t         type; // could be OP or CMD
	char*           data; // could be command name with its args or a filename
	struct s_tok*   next; 
}                   t_tok;
```
Lets show with a simple example how to tokenize a command line:
```
minish> $ ls | cat -e | rev

tokens: [type:CMD, data:(ls)][type:OP:PIPE, data:NULL][type:CMD, data:(cat -e)][type:OP:PIPE, data:NULL][type:CMD, data:(rev)]
```
As we can see the tokens has two types: operator or cmd. The operator used to split a job (the pipes) have no data, however, the redirections has the filename in data like:
```
minish> $ ls > file

tokens: [type:CMD, data:(ls)][type:OP:REDIR_GR, data:(file)]
```
Besides, the separtors of the command lines will be used for conditional calculation (if separators are "&&" or "||"). For example, imagine an input command with some separators, for each separator a new BST will be created and exectuted. The conditional return (true or false) of the command(s) executed on the bst will be stored during the bst execution. And then, after the execution is easy to do conditional calculation. The parentheses are only used for conditional calculation, they haven't effect on anything else:
```
minish> $ CMD_LINE && CMD_LINE || (CMD_LINE || CMD_LINE) || CMD_LINE ; CMD_LINE
             ^           ^             ^          ^             ^         ^
	     |           |             |          |             |         |
	 1 BST         1 BST         1 BST        1 BST        1 BST    always exec
     always exec      exec if       exec if       exec if     exec if
                      prev=true     prev=false    prev=false   parentheses=false
```

## Parsing

As explained above, the return of the lexer-parser are a linked-list of tokens. This tokens represent a command line and this tokens can be of two types: CMD for commands and OP for operators. Using this tokens, a binary search tree (BST) can be built to ensure a smooth execution.
This is the struct of a bst node:
```
typedef struct      	s_bst
{
	void*           a; // child left
	void*           b; // child right
	t_tok_t         type;
}                   	t_bst;
```
There are 3 possible cases to handle:
- Command line has at least a pipe
- Command line hasn't a pipe but has at least one redirection
- Command line hasn't pipe or redirection

To handle the last case a BST ins't needed, is enought with a single node. But to handle the other cases a BST is going to be built.
To get started, lets explain how to build a bst with a at least a pipe with an example:
For execute the following command line 
```
minish> $ echo -n "This is an example" | cat -e > file1 | cat < file1 > file2
```

who's tokens will be:
```
[type:CMD, data:(echo -n "This is an example")][type:OP:PIPE, data:NULL][type:CMD, data:(cat -e)][type:OP:REDIR_GR, data:(file1)][type:OP:PIPE, data:NULL][type:CMD, data:(cat)][type:OP:REDIR_LE, data:(file1)][type:OP:REDIR_GR, data:(file2)]
```

All the right childs (starting in the root of the BST) are operators type PIPE except the last right node, this can be a redirection or a simple command. The left nodes (starting of the root and in each right node) are the commands and/or redirections between the pipes. So for the given command line the BST must be:
```
	root-->	[ | ]
	       /     \
	     /	       \
     [echo -n           [ | ]
    "This is an	       /     \
      example"]      /         \
		   [ > ]        [ < ]
		  /    \        /   \
		 /      \      /     \
	     [cat -e] [file1] [ > ]   [file1]
	     	              /   \
			     /     \
			 [ cat ]  [file2]
			 
```
Build a bst for a command line without pipe but having at least a redirection is like the last right node and its left node. For the given command line 
```
minish >$ cat < file1 > file2
```
the result BST will be:
```
     root--> [ < ]
             /   \
	    /     \
	   [ > ]  [file1]
	   /   \
	  /     \
     [ cat ]   [file2]
```

## Expansion
TODO

## Execution

The execution is very easy with a BST built, the following struct is used to handle the execution:
```
typedef struct		s_exec
{
	int		fds[3]; // executions fds (fds[0] = STDIN, fds[1] = STDOUT, fds[2] is used to store pipe fds
	char		handle_dup; // used only for handle the case where there's at least one pipe and then at least one redirection
	t_executable	exec; // pointer to the builting or execve (with its arguments) function
	const char*	execution_path; // execve argument
	int		ac; // number of command arguments
    	char*const*	av; // pointer to command arguments
    	char*const*	ep; // pointer to environment
}			t_exec; 
```

Using "fds" to store the fd of stdin and stdout execute is pretty easy. Just have to init fds and set it values to the standard values:
```
t_exec	info;

info = (t_exec){.fds[0]=0, .fds[1]=1, .fds[2]=2};

```
Then, just have to overwrite the "fds" values and execute. Let's continue with the previous example:
```
                               // fds = {0, 1, 2}
	root-->	[ | ]          // overwrite stdout with pipe read and store pipe write, now fds = {pipe_read, 1, pipe_write}
	       /     \
	     /	       \
     [echo -n           [ | ]          // for the left branch execute echo -n "This is an example", in fds = {pipe_read, 1, pipe_write}
    "This is an	       /     \          // for the right brach, overwrite sdtin with fds[2] (pipe's write fd), fds = {pipe_read, pipe_write, pipe_write}
      example"]      /         \
		   [ > ]        [ < ]          // for the cat -e branch overwrite sdtout, fds = {file1, pipe_write, pipe_write}
		  /    \        /   \          // for the last right branch, fds = {0, file1, pipe_write}
		 /      \      /     \
	     [cat -e] [file1] [ > ]   [file1]          // for the cat-e branch the fds = {file1, pipe_write, pipe_write}, execute "cat -e" to the fds
	     	              /   \                    // for the last right branch overwrite stdout, fds = {file2, file1, pipe_write}
			     /     \
			 [ cat ]  [file2]          // execute cat in fds = {file2, file1, pipe_write}

// Don't forget that fds[2] is only for pipe's write fd! Its value doesn't affect a cmd exectution. 
```

# Specifications

## Key Bindings
TODO

## Token Types
| Type 				| Regular Expression 											| Comment 															|
|------				|--------------------											|---------															|
| TOKENS 			| ( IFS TOKEN IFS SEP )* 										| TOKENs separated by IFS and other separators 						|
| TOKEN 			| CMD IFS OPERATION* 											| Command followed by multiple operations 							|
| CMD 				| SUBSHELL \| CMD_SIMPLE 										| Subshell or command  												|
| SUBSHELL 			| '(' TOKENS ')' 												| Tokens wrapped in parenthesis 									|
| OPERATION 		| OPERATOR IFS CMD 												| Operator followed by command										|
| OPERATOR 			| "\|" \| "\|\|" \| "&&" 										| PIPE, OR or AND operator 											|
| CMD_SIMPLE 		| ( IFS ( PARAM \| INLINE ) )* 									| Multiple params or inline operations separated by IFS 			|
| PARAM 			| ( PARAM_QUOTED \| PARAM_SIMPLE )* 							| Mutliple quoted or simple params 									|
| PARAM_QUOTED 		| PARAM_ESCAPED \| PARAM_SQUOTED \| PARAM_DQUOTED 				| Escaped or quoted params 											|
| PARAM_ESCAPED 	| ( '\\\\' char )* 												| Multiple chars preceded by backslash 								|
| PARAM_SQUOTED 	| '\\'' ( char - \\' )* '\\'' 									| Multiple chars wrapped in single quotes, escaped by backslash 	|
| PARAM_DQUOTED 	| '"' ( ( char - ["\\"\\\\"] ) \| ( '\\' ["\\"\\\\"] ) )* '"' 	| Multiple chars wrapped in double quotes, escaped by backslash 	|
| PARAM_SIMPLE 		| ( char - [" \\t\\n\|&;()<>'\\"\\\\"] )* 						| Multiple chars that aren't special 								|
| INLINE 			| REDIR \| HEREDOC 												| Redirection or here-document 										|
| REDIR 			| REDIR_OPERATOR PARAM 											| Redirection operator followed by parameter 						|
| REDIR_OPERATOR 	| ">" \| ">>" \| "<" 											| Single and double redirections 									|
| HEREDOC 			| HEREDOC_OPERATOR PARAM 										| Here-document operator followed by parameter 						|
| HEREDOC_OPERATOR 	| "<<"															| Here-document operator 											|
| IFS 				| ( ' ' \| [\\t .. \\r] )*										| Multiple $IFS or white-spaces if unset 							|

## Expansion Types
TODO

## Built-in Commands
TODO

## Job Control

Job control is the ability to selectively stop (suspend) the execution of processes and continue (resume) their execution later point.
First of all i would like to clarify that this part is not part of the subject. However, we have implemented this feature because we found it interesting and above all because the knowledge that implementing it would bring us. And indded it was a great decison. We decided to not use functions that aren't allowed in the suject for this part. We have emulated the behavior of Bash without using syscalls like: setpgid, killpg, tctgetpgrp, getsid, getpid, tcgetsid, ioctl and (not as useful for our case) clone.

First of all lets start with a global Unix description of threads, processes, process groups and sessions. 

A session contain a number of process groups, and a process group contain a number of process, and each process contain a number of threads.

A session can also have a controlling tty but it was not possible to implement it with the allowed fuctions (like the threads).

All these objects have an id: session ID, process group ID, process ID, thread ID. In our case ther are only 1 session who has the process ID of "minishell" but each group and processes has an ID.

### I) Process

A process is traditionally started using fork or clone, this creates a child as a duplicate of its parent. Parent and child are identical in almost all respects. The parent will learn information about its childs like their process ID
or their termination receiving a SIGCHLD signal (deepened later). A process normal termination is when the process exits or returns in the main.

A process can be run in foreground (having the same group ID of its terminal, in our same processes who are waited when they have a waitable state) or can run in the background (having a diferent group ID of its terminal, in our case processes who aren't waited).

A process that has terminated but has not yet been waited is a zombie process. When a process end its sends a SIGCHLD to its parent who is by default not handled.

This is the struct used for handling processes:
```
typedef struct			s_process
{
	pid_t			pid;		// process ID
	int			wstatus;	// wait status
	unsigned char		flags;		// local status
	char*const		*data;		// name + args
	struct s_process	*next;		// next in lkd-lst
	struct s_process	*prev;		// prev in lkd-lst
	int			ret;		// process return
}				t_process;
```

### II) Process groups

Each process is member of a unique process group, like the processes each process group has its group process ID. When the first process is created its process ID becomes the ID of its process group (also know as leader process). Multiples pipes creates multiples processes in the same process group but separators creates multiple process groups.

Using the job control features is possible to stop a process group and resume it later in the background or in the foreground, to send it signals, to wait for it termination, or disown it (will no recive SIGHUP at the shell termination, see later).

This is the group structure we used:
```
typedef struct			s_group
{
	t_process		*active_processes;	// procesess
	t_process		*nil;			// control node (next is head, prev is tail)
	struct s_group		*next			// next in lkd-lst
	struct s_group		*prev;			// prev in lks-lst
	char			**input;		// input line
}				t_group;
```
Before continue i want to clarify than background processes can read and write from respectivelly stdin and stdout from the parent. Unfortunaly, tty control requires the use of tcsetpgrp and ioctl to be handled.

### III) Sesions

Each group is member of a unique session. We have a unique session who has the process ID of minishell executable.

This is how all works:
```
typedef struct			s_session
{
	int			st;		// return status
	t_group			*groups;	// all the groups
	t_group			*nil;		// control node (next is head prev is tail)
	t_history		*hist;		// history lkd-list
	t_background		*zombies;	// background groups
	t_deadzombie		*dead_zombies; 	// use for print zombies termination
	char			**input_line;
	size_t			input_line_index; 
	unsigned char		exit_count;	// exit control
	char			*name;		// executable name
	t_env			*env;		// local environment
	char			flags;		// local status
}				t_session;
```

The session is global and is defined in all minishell. It uses "groups" to have an index of all the stopped or background groups (other group aren't storred) and "hist", "zombies", and "deadzombie" as support.

There their definitions:
```
/* This is used as a stack and a minimalist list */

typedef struct			s_history
{
	t_group			*group;	// group reference
	struct s_history	*next;
}				t_history;

/* This is used as a stack and a minimalist list (used to iterate in asycronous over 
	the background proceses when SIGCHLD is called)*/

typedef struct			s_background
{
	t_group			*background_group;	// group reference
	bool			exited;			// status
	struct s_background	*next;
}						t_background;

/* This is used as a queue and a minimalist list */

typedef struct			s_deadzombie
{
	t_process		*deadzombie;		// group leader to print reference
	struct s_deadzombie	*next;
}				t_deadzombie;
```

### IV) Builtins and features

To stop a process witout a controlling tty signals SIGSTOP and SIGTSTP can be used. SIGSTOP can't be ignored and SIGTSTP and is send when ctrl^Z is typed in the terminal. A process can be resume receiving SIGCONT.

Futhermore, we handle SIGINT (sent when ctrl^C is typed in the terminal), who requires the termination of the current process execution, SIGQUIT (sent when ctrl^\\ is typed on the terminal), who cuases a coredump in the current execution. And of corse, we handle SIGTERM and SIGHUP who makes minishell terminate but with time to free all the ressources and exit clean.

When minishell terminates (if not receives SIGKILL) it will send SIGHUP to all the stopped or running processes and then continue them. This is not applied to disown processes.

Now the core of the job control is explained lets see how to interract with it. Job control builtins has been implemented to allow the user to interact with all the listed before job control features.


Note: job specification (jobspec) are explained later, ```"..."``` means any number of arguments and the options and follow the following formats:```"-srl"``` ```"-s -r -l"```.

#### bg [no args] | [jobspec]
- Without arguments resumes in the background the current stopped background group process.
- With a jobspec specified resumes the jobspec group of this jobspec.
- Returns non zero if the jobspec is not found else returns 0.

#### fg [no args] | [jobspec]
- Without arguments resumes in the foreground the current stopped or running in the background group process.
- With a jobspec spefied resumes the josbpec group if is possible.
- Return what its foreground command returns.
- Return no zero if the jobspec is not found.

#### jobs [-lrs...] [jobspec] | [no args]
- List the active groups witout arguments.
- If jobspecs are given list the targeted jobspecs.
- '-l' gives additional information.
- '-r' list only running groups.
- '-s' list only the stopped groups.

#### kill [-sigspec] | [-signum] | [no signum or sigspec] [josbpecs...] | [pid...] or kill -l [signum...]
- Send a signal (sigspec or signum) to the process group (jobspec or pid).
- Note: pid is sent to all the group if is sent to a member.
- SIG prefix is optional in sigspec.
- If signum or sigspec are missing SIGTERM is sent.
- '-l' option without jobspec or pid list all the avalaible signals.
- '-l' option with signum print the correspondent signals.
- '-L' can be use insted of '-l'.
- return 0 if the signal was sent succesfully.

#### wait [-fn...] [jobspec] | [pid] | [no arg]
- Wait until the process group exits.
- If jobspec, jobspec is the target else the current is waited.
- '-n' wait for a single job and return its return status..
- returns the exit status of the previous group waited

#### disown [-ar...] | [-h...] [jobspecs...] | [pids...]
- Disown groups will not receive SIGHUB when minishell terminates
that means they could still run in the backgroud after minishell has exited.
- Disown a process mean delete it form the groups list, no operations can be done to undone that.
- '-r' remove the running groups and restrict operations in stopped groups.
- '-a' disown all the groups.
- '-h' the exeption, doesn't remove and allow operations to groups but mark them and they will no recive SIGHUB at the termation of minishell.

#### Jobspec specification
- To reffer to the current group use: ```%, %% or %+```.
- To reffer to the previous of the current use ```%-``` (reffers to the current if theres only 1 group).
- To reffer to a group using its jobspec index use ```%index``` where index is an integer. Index starts with 1.
- To reffer to a group using its name use ```%name``` where name is a string of chars. Return error name if is ambigous.
- To reffer to a group using a pattern use ```%?pattern``` where pattern is a string of chars, Return error if pattern is ambigous.
- To reffer to a pid use ```pid```where pip is an integer. Applies to all the memebers of the group of the found process.
