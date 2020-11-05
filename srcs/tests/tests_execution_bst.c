/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_execution_bst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 00:34:56 by pablo             #+#    #+#             */
/*   Updated: 2020/11/05 04:22:07 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <separators.h>
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
	else
		return (NULL);
}


/*
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
*/

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
	//char*	data17[2] = {"pwd", NULL};
	//char*	data18[3] = {"echo", "This is another test", NULL};
	//char*	data19[3] = {"echo", "This is a test", NULL};

	// test 9, 10, 11
	char* data20[3] = {"echo", "true", NULL}; // 20 first
	char* data21[2] = {"pwd", NULL}; // 21 second
	(void)data20;
	(void)data21;

	// other tests
	char* _true[3] = {"echo", "true", NULL};
	char* _false[2] = {"pwd", NULL};

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

	// works: test8

	// test8 : random bullshit
	//t_tok t8test11 = (t_tok){.data="88test88", 			.type=REDIR_GR, .next=NULL};
	//t_tok t8test10 = (t_tok){.data="test8", 			.type=REDIR_GR, .next=&t8test11};
	//t_tok t8test9 = (t_tok){.data=data17,				.type=CMD, 		.next=&t8test10};
	//t_tok t8test8 = (t_tok){.data=NULL, 				.type=PIPE, 	.next=&t8test9};
	//t_tok t8test7 = (t_tok){.data="22out22", 			.type=REDIR_GR, .next=&t8test8};
	//t_tok t8test6 = (t_tok){.data="out2", 				.type=REDIR_GR, .next=&t8test7};
	//t_tok t8test5 = (t_tok){.data=data18,				.type=CMD, 		.next=&t8test6};
	//t_tok t8test4 = (t_tok){.data=NULL, 				.type=PIPE, 	.next=&t8test5};
	//t_tok t8test3 = (t_tok){.data="11out11", 			.type=REDIR_GR, .next=&t8test4};
	//t_tok t8test2 = (t_tok){.data="out1", 				.type=REDIR_GR, .next=&t8test3};
	//t_tok test8 = (t_tok){.data=data19, 				.type=CMD, 		.next=&t8test2};

	// works: test9

	// test9: SEMICOLON
	//t_tok	t9test3 = (t_tok){.data=data21,					.type=CMD,			.next=NULL};
	//t_tok	t9test2 = (t_tok){.data=NULL,					.type=SEMICOLON,	.next=&t9test3};
	//t_tok	test9 = (t_tok){.data=data20,					.type=CMD,			.next=&t9test2};

	// works: test10: true && flase AND flase and true

	// test10: AND
	//t_tok	t10test3 = (t_tok){.data=data21,				.type=CMD,			.next=NULL};
	//t_tok	t10test2 = (t_tok){.data=NULL,					.type=AND,			.next=&t10test3};
	//t_tok	test10 = (t_tok){.data=data20,					.type=CMD,			.next=&t10test2};

	// works: test11: flase || true AND true and true || false

	// test11: OR
	//t_tok	t11test3 = (t_tok){.data=data21,				.type=CMD,			.next=NULL};
	//t_tok	t11test2 = (t_tok){.data=NULL,					.type=OR,			.next=&t11test3};
	//t_tok	test11 = (t_tok){.data=data20,					.type=CMD,			.next=&t11test2};

	// works: test12 false && false ; false AND true && false ; false

	// test12: AND-SEMICOLON
	//t_tok	t12test5 = (t_tok){.data=_false,				.type=CMD,			.next=NULL};
	//t_tok	t12test4 = (t_tok){.data=NULL,					.type=SEMICOLON,	.next=&t12test5};
	//t_tok	t12test3 = (t_tok){.data=_false,					.type=CMD,			.next=&t12test4};
	//t_tok	t12test2 = (t_tok){.data=NULL,					.type=AND,			.next=&t12test3};
	//t_tok	test12 = (t_tok){.data=_true,					.type=CMD,			.next=&t12test2};

	// works: test13 true || true ; false AND flase || true ; false

	// test13: OR-SEMICOLON
	//t_tok	t13test5 = (t_tok){.data=_false,					.type=CMD,			.next=NULL};
	//t_tok	t13test4 = (t_tok){.data=NULL,					.type=SEMICOLON,	.next=&t13test5};
	//t_tok	t13test3 = (t_tok){.data=_true,				.type=CMD,			.next=&t13test4};
	//t_tok	t13test2 = (t_tok){.data=NULL,					.type=OR,			.next=&t13test3};
	//t_tok	test13 = (t_tok){.data=_true,					.type=CMD,			.next=&t13test2};

	// works: cmd ; cmd ; AND cmd ; cmd  ; cmd

	// test14: SEMICOLON-SEMICOLON
	//t_tok	t14test5 = (t_tok){.data=_false,				.type=CMD,			.next=NULL};
	//t_tok	t14test4 = (t_tok){.data=NULL,					.type=SEMICOLON,	.next=&t14test5};
	//t_tok	t14test3 = (t_tok){.data=_false,				.type=CMD,			.next=&t14test4};
	//t_tok	t14test2 = (t_tok){.data=NULL,					.type=SEMICOLON,	.next=&t14test3};
	//t_tok	test14 = (t_tok){.data=_true,					.type=CMD,			.next=&t14test2};

	// works: true && (flase && true && true) && true AND false && () && flase AND true && (true && true) && true

	// test15: AND-PARENTHESES + AND IN PARENTHESES
	//t_tok	t15test11 = (t_tok){.data=_true,				.type=CMD,			.next=NULL};
	//t_tok	t15test10 = (t_tok){.data=NULL,					.type=AND,			.next=&t15test11};
	//t_tok	t15test9 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t15test10};
	//t_tok	t15test8 = (t_tok){.data=_false,				.type=CMD,			.next=&t15test9};
	//t_tok	t15test7 = (t_tok){.data=NULL,					.type=AND,			.next=&t15test8};
	//t_tok	t15test6 = (t_tok){.data=_true,					.type=CMD,			.next=&t15test7};
	//t_tok	t15test5 = (t_tok){.data=NULL,					.type=AND,			.next=&t15test6};
	//t_tok	t15test4 = (t_tok){.data=_true,					.type=CMD,			.next=&t15test5};
	//t_tok	t15test3 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t15test4};
	//t_tok	t15test2 = (t_tok){.data=NULL,					.type=AND,			.next=&t15test3};
	//t_tok	test15 = (t_tok){.data=_true,					.type=CMD,			.next=&t15test2};

	// works: false || (true || flase || false) || false AND true || () || false AND false || (flase || false) || false

	// test16: OR-PARENTHESES + OR IN PARENTHESES
	//t_tok	t16test11 = (t_tok){.data=_true,					.type=CMD,		.next=NULL};
	//t_tok	t16test10 = (t_tok){.data=NULL,					.type=OR,			.next=&t16test11};
	//t_tok	t16test9 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t16test10};
	//t_tok	t16test8 = (t_tok){.data=_true,					.type=CMD,		.next=&t16test9};
	//t_tok	t16test7 = (t_tok){.data=NULL,					.type=OR,			.next=&t16test8};
	//t_tok	t16test6 = (t_tok){.data=_false,					.type=CMD,		.next=&t16test7};
	//t_tok	t16test5 = (t_tok){.data=NULL,					.type=OR,			.next=&t16test6};
	//t_tok	t16test4 = (t_tok){.data=_false,					.type=CMD,		.next=&t16test5};
	//t_tok	t16test3 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t16test4};
	//t_tok	t16test2 = (t_tok){.data=NULL,					.type=OR,			.next=&t16test3};
	//t_tok	test16 = (t_tok){.data=_false,					.type=CMD,			.next=&t16test2};

	// test17: AND-PARENTHESES + OR AND IN PARENTHESES

	// works: test17
	
	//t_tok	t17test11 = (t_tok){.data=_true,				.type=CMD,		.next=NULL};
	//t_tok	t17test10 = (t_tok){.data=NULL,					.type=OR,			.next=&t17test11};
	//t_tok	t17test9 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t17test10};
	//t_tok	t17test8 = (t_tok){.data=_false,				.type=CMD,			.next=&t17test9};
	//t_tok	t17test7 = (t_tok){.data=NULL,					.type=OR,			.next=&t17test8};
	//t_tok	t17test6 = (t_tok){.data=_false,				.type=CMD,			.next=&t17test7};
	//t_tok	t17test5 = (t_tok){.data=NULL,					.type=OR,			.next=&t17test6};
	//t_tok	t17test4 = (t_tok){.data=_false,				.type=CMD,			.next=&t17test5};
	//t_tok	t17test3 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t17test4};
	//t_tok	t17test2 = (t_tok){.data=NULL,					.type=AND,			.next=&t17test3};
	//t_tok	test17 = (t_tok){.data=_true,					.type=CMD,			.next=&t17test2};

	// test18: MULTIPLE PARENTHESES: test cmd && (((cmd && cmd) && cmd && cmd) && cmd && cmd) && cmd #2
								// and cmd && (cmd && cmd && (cmd && cmd ...)) #1 [done!]
								// check starting par without cond #3
								// end par #4 [done!]
	
	t_tok	t18test21 = (t_tok){.data=_false,				.type=CMD,			.next=NULL};
	t_tok	t18test20 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test21};
	t_tok	t18test19 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t18test20};
	t_tok	t18test18 = (t_tok){.data=_false,				.type=CMD,			.next=&t18test19};
	t_tok	t18test17 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test18};
	t_tok	t18test16 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t18test17};
	t_tok	t18test15 = (t_tok){.data=_true,				.type=CMD,			.next=&t18test16};
	t_tok	t18test14 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test15};
	t_tok	t18test13 = (t_tok){.data=NULL,					.type=CLOSE_PAR, 	.next=&t18test14};
	t_tok	t18test12 = (t_tok){.data=_true,				.type=CMD,			.next=&t18test13};
	t_tok	t18test11 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test12};
	t_tok	t18test10 = (t_tok){.data=_true,				.type=CMD,			.next=&t18test11};
	t_tok	t18test9 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t18test10};
	t_tok	t18test8 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test9};
	t_tok	t18test7 = (t_tok){.data=_true,					.type=CMD,			.next=&t18test8};
	t_tok	t18test6 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t18test7};
	t_tok	t18test5 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test6};
	t_tok	t18test4 = (t_tok){.data=_true,					.type=CMD,			.next=&t18test5};
	t_tok	t18test3 = (t_tok){.data=NULL,					.type=OPEN_PAR,		.next=&t18test4};
	t_tok	t18test2 = (t_tok){.data=NULL,					.type=AND,			.next=&t18test3};
	t_tok	test18 = (t_tok){.data=_true,					.type=CMD,			.next=&t18test2};
	(void)test18;

	// at the end search how a semicolon inter parentheses affects the parentheses return value #5
	
	t_term	term;
	ft_bzero(&term, sizeof(term));
	exec(&test18, &term);
}
