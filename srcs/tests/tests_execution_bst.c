/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_execution_bst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 00:34:56 by pablo             #+#    #+#             */
/*   Updated: 2020/11/04 22:03:11 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <stdio.h>

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
    if (code == CMD)
	{
        return "CMD";
	}
	if (code == FILENAME)
		return "FILENAME";
	else
		return (NULL);
}


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

void		tests()
{
	// test1
	// char* data1[3] = {"echo", "This is a test", NULL};

	// test2
	//char* data2[3] =  {"echo" ,"This is a test", NULL};

	// test3
	//char*	data3[2] = {"pwd", NULL};
	//char*	data4[2] = {"echo a", NULL};

	// test4
	//char*	data5[2] = {"pwd", NULL};
	//char*	data6[3] = {"echo", "This is another test", NULL};
	//char*	data7[3] = {"echo", "This is a test", NULL};

	// test5
	//char* data8[2] = {"pwd", NULL};
	//char* data9[3] = {"echo", "third pipe", NULL};
	//char* data10[3] = {"echo", "second pipe", NULL};
	//char* data11[3] = {"echo", "first pipe", NULL};

	// test6
	//char* data12[2] = {"pwd", NULL};
	//char* data13[3] = {"echo", "this is a test", NULL};
	
	// test7
	//char*	data14[2] = {"pwd", NULL};
	//char*	data15[3] = {"echo", "This is another test", NULL};
	//char*	data16[3] = {"echo", "This is a test", NULL};

	// test8
	char*	data17[2] = {"pwd", NULL};
	char*	data18[3] = {"echo", "This is another test", NULL};
	char*	data19[3] = {"echo", "This is a test", NULL};

	// works: test1

	// test1 : minish> $ echo a			SIMPLE CMD
	//t_tok test1 = (t_tok){.data=data1, .type=CMD, .next=NULL};

	// works: test2

	// test2 : minish> $ ls > test2		SIMPLE REDIRECTION
	// t_tok t2test2 = (t_tok){.data="test2", 				.type=REDIR_GR, .next=NULL};
	// t_tok test2 = (t_tok){.data=data2, 					.type=CMD, 		.next=&t2test2};
	
	// works: test3

	// test3 : minish> $ ls | cat -e	SIMPLE PIPE
	//t_tok t3test3 = (t_tok){.data=data3,					.type=CMD, 		.next=NULL};
	//t_tok t3test2 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t3test3};
	//t_tok test3 = (t_tok){.data=data4, 					.type=CMD, 		.next=&t3test2};

	// works: test4

	// test4 : minish> $ ls | cat -e |  rev		DOUBLE PIPE
	//t_tok t4test5 = (t_tok){.data=data5, 					.type=CMD, 		.next=NULL};
	//t_tok t4test4 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t4test5};
	//t_tok t4test3 = (t_tok){.data=data6,					.type=CMD, 		.next=&t4test4};
	//t_tok t4test2 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t4test3};
	//t_tok test4 = (t_tok){.data=data7, 					.type=CMD, 		.next=&t4test2};


	// works: test5

	// test5 : minish> $ ls | cat -e | rev | cat -e	TRIPLE PIPE
	//t_tok t5test7 = (t_tok){.data=data8,					.type=CMD, 		.next=NULL};
	//t_tok t5test6 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t5test7};
	//t_tok t5test5 = (t_tok){.data=data9, 					.type=CMD, 		.next=&t5test6};
	//t_tok t5test4 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t5test5};
	//t_tok t5test3 = (t_tok){.data=data10,					.type=CMD, 		.next=&t5test4};
	//t_tok t5test2 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t5test3};
	//t_tok test5 = (t_tok){.data=data11, 					.type=CMD, 		.next=&t5test2};


	// works: test6 but redir fd is 0...,BUT WORKS!

	// test6 : minish> $ ls | cat -e > test6		SIMPLE PIPE WITH SIMPLE REDIRECTION
	//t_tok t6test4 = (t_tok){.data="test6", 				.type=REDIR_GR, .next=NULL};
	//t_tok t6test3 = (t_tok){.data=data12,					.type=CMD, 		.next=&t6test4};
	//t_tok t6test2 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t6test3};
	//t_tok test6 = (t_tok){.data=data13, 					.type=CMD, 		.next=&t6test2};

	// works: test7
	
	// test7 : minish> $ ls > out | cat < out | cat -e		SIMPLE REDIRECTIONS INTERPIPE
	//t_tok t7test7 = (t_tok){.data=data14,					.type=CMD, 		.next=NULL};
	//t_tok t7test6 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t7test7};
	//t_tok t7test5 = (t_tok){.data="out2", 					.type=REDIR_GR, .next=&t7test6};
	//t_tok t7test4 = (t_tok){.data=data15,					.type=CMD, 		.next=&t7test5};
	//t_tok t7test3 = (t_tok){.data=NULL, 					.type=PIPE, 	.next=&t7test4};
	//t_tok t7test2 = (t_tok){.data="out1", 					.type=REDIR_GR, .next=&t7test3};
	//t_tok test7 = (t_tok){.data=data16, 					.type=CMD, 		.next=&t7test2};

	// test8 : random bullshit
	t_tok t8test11 = (t_tok){.data="88test88", 			.type=REDIR_GR, .next=NULL};
	t_tok t8test10 = (t_tok){.data="test8", 			.type=REDIR_GR, .next=&t8test11};
	t_tok t8test9 = (t_tok){.data=data17,				.type=CMD, 		.next=&t8test10};
	t_tok t8test8 = (t_tok){.data=NULL, 				.type=PIPE, 	.next=&t8test9};
	t_tok t8test7 = (t_tok){.data="22out22", 			.type=REDIR_GR, .next=&t8test8};
	t_tok t8test6 = (t_tok){.data="out2", 				.type=REDIR_GR, .next=&t8test7};
	t_tok t8test5 = (t_tok){.data=data18,				.type=CMD, 		.next=&t8test6};
	t_tok t8test4 = (t_tok){.data=NULL, 				.type=PIPE, 	.next=&t8test5};
	t_tok t8test3 = (t_tok){.data="11out11", 			.type=REDIR_GR, .next=&t8test4};
	t_tok t8test2 = (t_tok){.data="out1", 				.type=REDIR_GR, .next=&t8test3};
	t_tok test8 = (t_tok){.data=data19, 				.type=CMD, 		.next=&t8test2};

	t_term	term;
	ft_bzero(&term, sizeof(term));

	t_bst* root = bst(&test8);
	//print_bst(root);
	execute_bst(root, &term);
	//if (root)
	//	free_bst(root);
}
