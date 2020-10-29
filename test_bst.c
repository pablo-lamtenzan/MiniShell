/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_bst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 20:11:08 by pablo             #+#    #+#             */
/*   Updated: 2020/10/30 00:37:47 by pablo            ###   ########.fr       */
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

void    allocate_cmd_node(t_bst** curr, char* data)
{
	*curr = malloc(sizeof(t_bst));
	**curr = (t_bst){.a=data, .b=NULL, .type=CMD};
}

void   build_job(t_tok* tokens, t_bst** curr, t_tok* delim)
{
	// [db] #########################################################################################
				printf("BUILD JOB IS CALLED [%d] times!\n TARGET NODE IS [%p] (should be nul)\n",db2++, *curr);
	// [en db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    t_tok*  cp;

    cp = tokens;
    while (cp->next && cp->next != delim)
        cp = cp->next;
	if (!(*curr = malloc(sizeof(t_bst))))
        return ;

	// [db] #############################################################################################
    			printf("----------------------------------->TARGET NODE IS ALLOCATED! [%p]\n", *curr);
    			printf("New address of copy is [%p] operator is [%d]\n", cp, cp->type);
	// [db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    **curr = (t_bst){.a=NULL, .b=NULL, .type=cp->type};

	// [db] ###########################################################################################
    			printf("NODE TYPE = COPY TYPE! node type [%d]\n", (*curr)->type);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if (cp != tokens)
    {
		
		// [db] ##################################################################################################
        			printf("CHECK FOR NEXT CMD!\n");
        			printf("tokens is [%p], curr is [%p], curr->a is [%p] and cp is [%p]\n", tokens, *curr, (t_bst**)(*curr)->a, cp);
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        build_job(tokens, (t_bst**)&(*curr)->a, cp);
        (*curr)->b = malloc(sizeof(t_bst));
		*(t_bst*)(*curr)->b = (t_bst){.a=cp->data, .b=NULL, .type=cp->type | CMD};
    }
    else
    {
		allocate_cmd_node((t_bst**)&(*curr)->a, cp->data);
		allocate_cmd_node((t_bst**)&(*curr)->b, cp->data);

		// [db] #########################################################################################
					printf("NO MORE CMDS\n");
        			printf("ADDRESS OF CHILD A IS OF [%p]: [%p]\n", *curr, (*curr)->a);
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    }
	// [db] #################################################################################
    				printf("----------------------------------->ADDR AT THE END [%p]\n", *curr);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
}

int db = 0;

void build_bst(t_tok* tokens, t_bst **root, t_bst** curr)
{

	// [db] ###############################################################################
	    		printf("Curr addr is [%p] (should be nul)\n", *curr);
    			printf("BUILD BST count[%d]\ntoken addr[%p] {cp=tokens} \n", db++, tokens);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
    t_tok* cp;

    cp = tokens;
    while (cp->next && !(cp->type & PIPE))
        cp = cp->next;

	// [db] ############################################################################
    			printf("NEW ADDR OF CP is [%p] operator [%d]\n", cp, cp->type);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if ((!*curr || !(cp->type & PIPE)) && !(*curr = malloc(sizeof(t_bst))))
        return ;

	// [db] #############################################################
    			printf("New BST NODE ALLOCATED! [%p]\n", *curr);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    **curr = (t_bst){.a=NULL, .b=NULL, .type=cp->type};
    if (!*root)
    {
        *root = *curr;

		// [db] ########################################################################
        		printf("Root is [%p] operator in root is [%d]\n", *root, (*root)->type);
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    }

	// [db] ##############################################################################
    			printf("New BST NODE [%p] type is [%d]\n", curr, (*curr)->type);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if ((*curr)->type & PIPE)
    {
		// [db] ###########################################################################################
        		printf("CALL buid_job in left node!\n");
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        build_job(tokens, (t_bst**)&(*curr)->a, cp);

		// [db] ############################################################################################
        		db2 = 0;
        		printf("······························>[%p] WAS ALLOCATED in BUILD_JOB (right node)\n", (*curr)->a);
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    }
    else
    {
		// [db] ######################################
        		printf("CALL build_job in right node!\n");
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        build_job(tokens, (t_bst**)&(*curr)->b, cp); // last job

		// [db] #########################################################################################
       			db2 = 0;
        		printf("..............................>[%p] WAS ALLOCATED in BUILD_JOB (letf node)\n", (*curr)->b);
        		printf("ADDRESS OF CHILD  \"A\" OF [%p] is: [%p]\n", ((t_bst*)(*curr)->b)->a);
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    }
	// [db] ##############################################################################################
    			printf("TOKENS ADDR IS [%p] operator is: [%d]\n", tokens, tokens ? tokens->type : 9999999999);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
    if (cp->next)
        build_bst(cp->next, root, (t_bst**)&(*curr)->b);
    else
        (*curr)->b = NULL;
    return ;
}

char* get_token_name(int code)
{
    if (code == 1)
        return "PIPE";
    if (code == 2)
        return "REDIR_GR";
    if (code == 4)
        return "REDIR_LE";
    if (code == 8)
        return "REDIR_DG";
    if (code == 16)
        return "SEMICOLON";
    if (code == 32)
        return "CMD";
}

void free_bst(t_bst* root)
{
	// [db] ########################################################################################
    			printf("free_bst input address: [%p] type: [%d]\n", root, root->type);
    			printf("IF RETURN IS [%d]\n", root && root->a && !(root->type & CMD));
	// [db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if (!(root->type & CMD));
    {
		// [db] ###########################################################################################################################################
        		printf("type is [%d][%s] !(root->type & CMD) = [%d] %s\n", root->type, get_token_name(root->type), !(root->type & CMD), (root->type & CMD) ?  "HOW THE FCK THE CODE JUMPS HERE IF THE CONDITION RETURNS 0???!!!" : "");
		// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        free_bst(root->a);
    }
    if (!(root->type & CMD))
        free_bst(root->b);

	// [db] ###########################################################
    printf("freed address is: [%p]\n", root);
	// [end db] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    free(root);
}

int main(int ac, char** av)
{
    int i = 1;
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
    t_bst* root = NULL;
    t_bst* tmp = NULL;
    build_bst(head, &root, (t_bst**)&tmp);
    printf("BST CREATED WITH SUCCES!\n");
    t_bst* cp = root;
	t_bst* remember;
	printf("GOES TO FREE NOW [%p]\n", root);
    while (cp->b)
    {
        printf("RIGHT BRANCH: operator code:[%d] operator name:[%s] operator addr:[%p]\n", ((t_bst*)cp)->type, get_token_name(((t_bst*)cp)->type), cp);
		remember = cp;
        while (cp->a )
		{
            printf("LEFT BRANCH: operator code:[%d] operator name:[%s] operator adrr:[%p]\n", ((t_bst*)cp)->type, get_token_name(((t_bst*)cp)->type), cp);
            if (cp->type & CMD)
                break ;
			cp = cp->a;
		}
        cp = remember->b;
    }
	printf("RIGHT BRANCH: operator [%d] [%p]\n", ((t_bst*)cp)->type, cp);
    printf("RIGHT BRANCH: operator [%d] [%p]\n", /*((t_bst*)cp->a)->type*/9090, cp->a);
	free_bst(root);
    return (0);
}