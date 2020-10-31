/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2_bst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 23:16:52 by pablo             #+#    #+#             */
/*   Updated: 2020/10/31 05:16:54 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

# define NONE       0
# define PIPE       1
# define REDIR_GR   2
# define REDIR_LE   4
# define REDIR_DG   8
# define SEMICOLON  16
# define CMD		32
# define FILENAME	64

typedef char    t_tok_t;

typedef struct  s_bst
{
    void*       a;
    void*       b;
    t_tok_t     type;
}               t_bst;

typedef struct s_tok
{
    t_tok_t     type;
    char*		data;
    struct s_tok* next;
}               t_tok;

t_bst*		build_bst(t_tok* tokens);

t_tok*		find_next_operator(t_tok* start, t_tok_t type)
{
	if (!start)
		return (start);
	while (start->next && !(start->type & type))
		start = start->next;
	return (start);
}

t_tok*		find_last_operator(t_tok* start, t_tok* end)
{
	t_tok*	last;

	last = start;
	while (last->next != end)
		last = last->next;
	if (last->type & CMD && start != last)
	{
		while (start->next != last)
			start = start->next;
	}
	else
		start = last;
	return (start);
}

int db2 = 0;

t_bst*		build_job(t_tok* tokens, t_tok* delim)
{
    t_tok* 	tk1;
	t_bst*	node;

	dprintf(2, "[BST]Loops [%d] times!\n", db2++);

	if (!(node = malloc(sizeof(t_bst))))
		return (NULL);

	// get the last operator
	tk1 = find_last_operator(tokens, delim);

	// set to the new node the operator type
	*node = (t_bst){.a=NULL, .b=NULL, .type=tk1->type};

	// if there's more redirection operators
	if (tk1 != find_next_operator(tokens, REDIR_GR | REDIR_LE | REDIR_DG))
	{
		// call itself and check for more redirections
		node->a = build_job(tokens, tk1);
		// put the filename on current b node
		node->b = malloc(sizeof(t_bst));
		if (node->b)
			*(t_bst*)node->b = (t_bst){.a=tk1->data, .b=NULL, .type=FILENAME};
	}

	// if there's not more redirection operators
	else
	{
		// current node a takes the first token (cmd name)
		if (tokens->type & CMD)
		{
			node->a = malloc(sizeof(t_bst));
			if (node->a)
				*(t_bst*)node->a = (t_bst){.a=tokens->data, .b=NULL, .type=CMD};
		}
		// current node b takes the next token (filename)
		if (tk1->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		{
			node->b = malloc(sizeof(t_bst));
			if (node->b)
				*(t_bst*)node->b = (t_bst){.a=tk1->data, .b=NULL,.type=FILENAME};
		}
	}
	return (node);
}

t_tok*		is_pipe_cmd(t_tok* start)
{
	t_tok*	cp;

	cp = start;
	while (cp->next)
	{
		if (cp->type & PIPE)
			return (NULL);
		cp = cp->next;
	}
	return (cp);
}

t_bst*		get_bst_root(t_tok* tokens)
{
	t_tok*	last_node;

	if ((last_node = is_pipe_cmd(tokens)))
		return (build_bst(tokens));
	else if (tokens->next)
		return (build_job(tokens, last_node));
	return (build_job(tokens, NULL));
}

int db1 = 0;

// not handles 1 node empty cmd
t_bst*		build_bst(t_tok* tokens)
{
    t_tok*	tk1;
	t_tok*	tk2;
	t_tok*	tk3;
    t_bst*	node;

	dprintf(2, "[BST]Loops [%d] times!\n", db1++);

    if (!(node = malloc(sizeof(t_bst))))
        return (NULL);

	// There's a pipe
	if ((tk1 = find_next_operator(tokens, PIPE))->type & PIPE)
		*node = (t_bst){.a=NULL, .b=NULL, .type=tk1->type};

	// There's no pipe but there's redirections
	else if ((tk3 = find_next_operator(tokens, REDIR_GR | REDIR_LE | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		*node = (t_bst){.a=NULL, .b=NULL, .type=tk3->type};

	// There's no pipe or redirection
	else
		*node = (t_bst){.a=NULL, .b=NULL, .type=CMD};

	// Build the branch a with the cmd abd possible redirections
    node->a = build_job(tokens, tk1);

	// If there's a pipe further, call itself and repeat starting in the next pipe
    if ((tk2 = find_next_operator(tk1->next, PIPE))->type & PIPE)
        node->b = build_bst(tk2);

	// If there's a redirection further, build redirection in node a starting in current b node
	else if ((tk3 = find_next_operator(tk1->next, REDIR_GR | REDIR_LE | REDIR_DG))->type & (REDIR_GR | REDIR_LE | REDIR_DG))
		node->b = build_job(tk3, tk2);
	
	// If there's no pipe or redirection further, build cmd in current node b
	else
		node->b = build_job(tk1->next, NULL);
	return (node);
}
// testing stuff
char* get_token_name(int code)
{
    if (code == PIPE)
        return "PIPE";
    if (code == REDIR_GR)
        return "REDIR_GR";
    if (code == REDIR_LE)
        return "REDIR_LE";
    if (code == REDIR_DG)
        return "REDIR_DG";
    if (code == SEMICOLON)
        return "SEMICOLON";
    if (code == CMD)
        return "CMD";
	if (code == FILENAME)
		return "FILENAME";
}

void free_bst(t_bst* root)
{
	printf("FREED NODE [%p]'s TYPE IS: code[%d][%s]\n", root, root->type, get_token_name(root->type));
	if (!(root->type & (CMD | FILENAME)))
	{
		free_bst(root->a);
		free_bst(root->b);
	}
	free(root);
}

/* ################################# TESTING #################################### */

void		print_bst(t_bst* root)
{
	//printf("BST NODE [%p]'s TYPE IS: code:[%d] type:[%s]\n", root, root->type, get_token_name(root->type));
	//if (root->type & (CMD | FILENAME))
	//	return ;
	//print_bst(root->a);
	//print_bst(root->b);
	t_bst* cp ;
	
	while (root && !(root->type & (CMD | FILENAME)))
	{
		dprintf(2, "[PRINT NODE] RIGHT BRANCH [%p]'s TYPE IS:[%d][%s]\n", root, root->type, get_token_name(root->type));
		cp = root;
		while (cp && !(cp->type & (CMD | FILENAME)))
		{
			dprintf(2, "[PRINT NODE] LEFT BRANCH [%p]'s TYPE IS:[%d][%s]\n", cp, cp->type, get_token_name(cp->type));
			cp = cp->a;
		}
		root = root->b;
	}
}

int main(int ac, char** av)
{
    int i = 0;
    t_tok* token;
    t_tok* head;
    t_tok* last;

    last = NULL;
    head = NULL;
    while (++i < ac)
    {
        token = malloc(sizeof(t_tok));
        if (!head)
            head = token;
        if (last)
            last->next = token;
        *token = (t_tok){.type=atoi(av[i]), .data=calloc(1, 1), .next=NULL};
        printf("CREATION OF TOKENS: TOKEN[%d] is [%d][%s]\n", i - 1, token->type, get_token_name(token->type));
        last = token;
        token = token->next;
    }
	t_bst* root = get_bst_root(head);
	t_bst*	pcp = root;
	print_bst(pcp);
	free_bst(root);
}