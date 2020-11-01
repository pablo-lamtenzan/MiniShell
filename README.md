# miniShell
### An implementation of a Shell-Bach from scratch by Clemax & pablo-lamtenzan

### Index
#### I) Usage
#### II) Lexer-Parser
#### III) BST
#### IV) Execution
#### V) Termcaps + env + idk what more (?)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## I) Usage
 to do
 
## II) Lexer-Parser
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
	char*           data; // could be command name with args or filename
	struct s_tok*   next; 
}                   t_tok;
```
Lets show with a simple example how to tokenize a command line:
```
minish> $ ls | cat -e | rev

tokens: [type:CMD, data:(ls)][type:OP:PIPE, data:NULL][type:CMD, data:(cat -e)][type:OP:PIPE, data:NULL][type:CMD, data:(rev)]
```
As we can see the tokens has two types: operator or cmd. The operator used to split a job (the pipes) have no data, however, the redirections have the filename in data like:
```
minish> $ ls > file

tokens: [type:CMD, data:(ls)][type:OP:REDIR_GR, data;(file)]
```
Besides, the separtors of the command lines will be used for conditional calculation (if separators are "&&" or "||"). For example, imagine an input command with some separators, for each separator a new BST will be created and exectuted. The conditional return (true or false) of the command(s) executed on the bst will be stored during the bst execution. And then, after the execution is easy to do conditional calculation. The parentheses are only used for conditional calculation, they haven't effect on anything else:
```
minish> $ CMD_LINE && CMD_LINE || (CMD_LINE || CMD_LINE) || CMD_LINE ; CMD_LINE
             ^           ^             ^          ^             ^         ^
	     |           |             |          |             |         |
	 1 BST         1 BST         1 BST        1 BST        1 BST    always exec
     always exec      exec if       exec if       exec if     exec if
                      prev=true     prev=false    prev=false   parentheses=true
```

## III) BST

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
[type:CMD, data:(echo -n "This is an example)][type:OP:PIPE, data:NULL][type:CMD, data:(cat -e)][type:OP:REDIR_GR, data:(file1)][type:OP:PIPE, data:NULL][type:CMD, data:(cat)][type:OP:REDIR_LE, data:(file1)][type:OP:REDIR_GR, data:(file2)]
```

All the right childs (starting in the root of the BST) are operators type PIPE except the last right node, this can be a redirection or a simple command. The left nodes (starting of the root and in each right node) are the commands and/or redirections between the pipes. So for the given command line the BST must be:
```
		[ | ]
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
             [ < ]
             /   \
	    /     \
	   [ > ]  [file1]
	   /   \
	  /     \
     [ cat ]   [file2]
```

## IV) Execution

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
t_exec *info;

*info = (t_exec){.fds[0]=0, .fds[1]=1, .fds[2]=2};
```
Then, just have to overwrite the "fds" values and execute. Let's continue with the previous example:
```
                           // fds = {0, 1, 2}
		[ | ]      // overwrite stdout with pipe read and store pipe write, now fds = {pipe_read, 1, pipe_write}
	       /     \
	     /	       \
     [echo -n           [ | ]    // overwrite sdtin with fds[2] (pipe write), fds = {pipe_read, pipe_write, pipe_write}
    "This is an	       /     \
      example"]      /         \
		   [ > ]        [ < ] // for the cat -e branch overwrite sdtout, fds = {file1, pipe_write, pipe_write}
		  /    \        /   \ // for the last right branch, fds = {0, file1, pipe_write}
		 /      \      /     \
	     [cat -e] [file1] [ > ]   [file1]    // for the cat-e branch the fds = {file1, pipe_write, pipe_write}, execute "cat -e" to the fds
	     	              /   \               // for the last right branch overwrite stdout, fds = {file2, file1, pipe_write}
			     /     \
			 [ cat ]  [file2] // execute cat in fds = {file2, file1, pipe_write}
			 
```
