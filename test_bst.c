/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_bst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 20:11:08 by pablo             #+#    #+#             */
/*   Updated: 2020/10/29 05:46:03 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

# define NONE       0
# define PIPE       1
# define REDIR_GR   2
# define REDIR_LE   4
# define REDIR_DG   8
# define SEMICOLON  16
# define CMD		32

# define LEFT       1
# define RIGHT      2

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

int db2 = 0;

void   build_job(t_tok* tokens, t_bst** curr, t_tok* delim)
{
    t_tok*  cp;

    cp = tokens;
    printf("BUILD JOB IS CALLED [%d] times!\n TARGET NODE IS [%p] (should be nul)\n",db2++, *curr);
    while (cp->next && cp->next != delim)
        cp = cp->next;
	if (!(*curr = malloc(sizeof(t_bst))))
        return ;
    printf("----------------------------------->TARGET NODE IS ALLOCATED! [%p]\n", *curr);
    printf("New address of copy is [%p] operator is [%d]\n", cp, cp->type);
    **curr = (t_bst){.a=NULL, .b=NULL, .type=cp->type};
    printf("NODE TYPE = COPY TYPE! node type [%d]\n", (*curr)->type);
    if (cp != tokens)
    {
        printf("CHECK FOR NEXT CMD!\n");
        printf("tokens is [%p], curr is [%p], curr->a is [%p] and cp is [%p]\n", tokens, *curr, (t_bst**)(*curr)->a, cp);
        build_job(tokens, (t_bst**)&(*curr)->a, cp);
        (*curr)->b = malloc(sizeof(t_bst));
		*(t_bst*)(*curr)->b = (t_bst){.a=cp->data, .b=NULL, .type=cp->type};
    }
    else
    {
        printf("NO MORE CMDS\n");
        (*curr)->a = malloc(sizeof(t_bst));
		*(t_bst*)(*curr)->a = (t_bst){.a=cp->data, .b=NULL, .type=CMD};
        (*curr)->b = malloc(sizeof(t_bst));
		*(t_bst*)(*curr)->b = (t_bst){.a=cp->data, .b=NULL, .type=CMD};
    }
    printf("----------------------------------->ADDR AT THE END [%p]\n", *curr);
}

int db = 0;

void build_bst(t_tok* tokens, t_bst **root, t_bst** curr)
{
    t_tok* cp;

    cp = tokens;
    printf("Curr addr is [%p] (should be nul)\n", *curr);
    printf("BUILD BST count[%d]\ntoken addr[%p] {cp=tokens} \n", db++, tokens);
    while (cp->next && !(cp->type & PIPE))
        cp = cp->next;
    printf("NEW ADDR OF CP is [%p] operator [%d]\n", cp, cp->type);
    if ((!*curr || !(cp->type & PIPE)) && !(*curr = malloc(sizeof(t_bst))))
        return ;
    printf("New BST NODE ALLOCATED! [%p]\n", *curr);
    **curr = (t_bst){.a=NULL, .b=NULL, .type=cp->type};
    if (!*root)
    {
        *root = *curr;
        printf("Root is [%p] operator in root is [%d]\n", *root, (*root)->type);
    }
    printf("New BST NODE [%p] type is [%d]\n", curr, (*curr)->type);
    if ((*curr)->type & PIPE)
    {
        printf("CALL buid_job in left node!\n");
        build_job(tokens, (t_bst**)&(*curr)->a, cp);
        db2 = 0;
        printf("······························>[%p] WAS ALLOCATED in BUILD_JOB (right node)\n", (*curr)->a);
    }
    else
    {
        printf("CALL build_job in right node!\n");
        build_job(tokens, (t_bst**)&(*curr)->b, cp); // last job
        db2 = 0;
        printf("..............................>[%p] WAS ALLOCATED in BUILD_JOB (letf port)\n", (*curr)->b);
    }
    printf("TOKENS ADDR IS [%p] operator is: [%d]\n", tokens, tokens ? tokens->type : 9999999999);
    if (cp->next)
        build_bst(cp->next, root, (t_bst**)&(*curr)->b);
    else
        (*curr)->b = NULL;
    return ;
}

void free_bst(t_bst* root)
{
	printf("addr to free is: [%p]\n", root);
    if (root && root->a);
        free_bst(root->a);
    if (root && root->b)
        free_bst(root->b);
    free(root);
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
        last = token;
        token = token->next;
    }
    t_bst* root = NULL;
    t_bst* tmp = NULL;
    build_bst(head, &root, (t_bst**)&tmp);
    printf("BST CREATED WITH SUCCES!\n");
    t_bst* cp = root;
	t_bst* remember;
	printf("GOES TO FREE NOW [%p]\n", root);
    while (cp->b && !(cp->type & CMD))
    {
        printf("RIGHT BRANCH: operator [%d] [%p]\n", ((t_bst*)cp)->type, cp);
		remember = cp;
		/*
        while (cp->a )
		{
            printf("LEFT BRANCH: operator [%d] [%p]\n", ((t_bst*)cp)->type, cp);
			cp = cp->a;
		}
		*/
        cp = remember->b;
    }
	printf("RIGHT BRANCH: operator [%d] [%p]\n", ((t_bst*)cp)->type, cp);
	free_bst(root);
    return (0);
}