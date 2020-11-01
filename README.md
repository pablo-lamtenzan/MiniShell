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
- Command line has a pipe
- Command line hasn't a pipe but at least one redirection
- Command line hasn't pipe or redirection

To handle the last case a bst ins't needed, is enought with a single node.

## IV) Execution
