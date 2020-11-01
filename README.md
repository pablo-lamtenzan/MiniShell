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
typedef struct      s_bst
{
	void*           a; // child left
	void*           b; // child right
	t_tok_t         type;
}                   t_bst;
```
There are 3 possible cases to handle:
- Command line has at least a pipe
- Command line hasn't a pipe but at least one redirection
- Command line hasn't pipe or redirection

To handle the last case a BST ins't needed, is enought with a single node. But to handle the other cases a BST is going to be built.
To get started, lets explain how to build a bst with a at least a pipe with an example:
For execute the following command line ```minish> $ echo -n "This is an example" | cat -e > file1 | cat < file1 > file2```

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
			 [ NULL ]  [file2]
			 
```
Build a bst for a command line without pipe but having at least a redirection is like the last right node and it's left node. For the given command line ```miniSH >$ cat < file1 > file2``` the result BST will be:
```
             [ < ]
             /   \
	    /     \
	   [ > ]  [file1]
	   /   \
	  /     \
     [ NULL ]   [file2]
```

## IV) Execution
