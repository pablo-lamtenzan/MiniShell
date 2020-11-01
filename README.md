# miniShell
### An implementation of Shell-Bach from scratch by Clemax & pablo-lamtenzan

### Index
#### I) Use
#### II) Lexer-Parser
#### III) BST
#### IV) Execution
#### V) Termcaps (?)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------

## I) Use
 to do
 
## II) Lexer-Parser
to do

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
For execute the following command line ```
minish> $ echo -n "This is an example" | cat -e > file1 | cat < file1 > file2```

who's tokens will be:
```[type:CMD, data:(echo -n "This is an example)][type:OP:PIPE, data:NULL][type:CMD, data:(cat -e)][type:OP:REDIR_GR, data:(file1)][type:OP:PIPE, data:NULL][type:CMD, data:(cat)][type:OP:REDIR_LE, data:(file1)][type:OP:REDIR_GR, data:(file2)]```

All the right childs starting in the root of the BST are operators type PIPE except the last right node, this can be a redirection or a simple command. The left nodes starting of the root and each right node are the commands and/or redirections between the pipes. So for the given command line the BST must be:
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
Build a bst for a command line without pipe but having at least a redirection is like the last right node and it's left node. For the given command line ```minish >$ cat < file1 > file2``` the result BST will be:
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

The execution is very easy having a BST built, the following struct is used to handle the execution:
```
typedef struct		s_exec
{
	int		fds[3]; // executions fds (fds[0] = STDIN, fds[1] = STDOUT, fds[2] is used to store pipe fds
	char		fidex_fds; // used only for handle the case where there's at least one pipe and then at least one redirection
	t_executable	exec; // pointer to the builting or execve (with its arguments) function
	const char*	execution_path; // execve argument
	int		ac; // number of command arguments
    char*const*		av; // pointer to command arguments
    char*const*		ep; // pointer to environment
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
     [echo -n           [ | ]    // overwrite sdtin with fds[2] (pipe write), fds = {0, pipe_write, pipe_write}
    "This is an	       /     \
      example"]      /         \
		   [ > ]        [ < ] // for the cat -e branch overwrite sdtout, fds = {file1, pipe_write, pipe_write}
		  /    \        /   \ // for the last right branch, fds = {0, file1, pipe_write}
		 /      \      /     \
	     [cat -e] [file1] [ > ]   [file1]    // for the cat-e branch the fds = {file1, pipe_write, pipe_write}, execute "cat -e" to the fds
	     	              /   \               // for the last right branch, fds = {file2, file1, pipe_write}
			     /     \
			 [ cat ]  [file2] // execute cat in fds = {file2, file1, pipe_write}
			 
```
