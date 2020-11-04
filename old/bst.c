/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 17:23:02 by plamtenz          #+#    #+#             */
/*   Updated: 2020/10/28 09:04:55 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <stdlib.h>

static void			assign_values(t_bst* target, t_cmd* cmds[2])
{
	if (cmds[0])
	{
		target->ac[0] = cmds[0]->ac;
		target->av[0] = cmds[0]->av;
	}
	else
	{
		target->ac[0] = 0;
		target->av[0] = NULL;
	}
	if (cmds[1])
	{
		target->ac[1] = cmds[1]->ac;
		target->av[1] = cmds[1]->av;
	}
	else
	{
		target->ac[1] = 0;
		target->av[1] = NULL;
	}
}

static t_bst*		new_node(const t_operator_t operator, t_cmd* cmds[2],
		t_bst* back)
{
	t_bst*			new;

	if (!(new = (t_bst *)malloc(sizeof(t_bst))))
		return (NULL);
	new->next = NULL;
	if (back)
		back->next = new;
	new->back = back;
	assign_values(new, cmds);
	new->operator = operator;
	ft_dprintf(2, "[bst][token] %s - %s [type=%d]\n",
		new->av[0] ? new->av[0][0] : NULL, new->av[1] ? new->av[1][0] : NULL, new->operator);
	return (new);
}

t_bst*				build_bst(t_operator* operators, t_cmd* cmds)
{
	t_bst*			tail;
	t_bst*			head;
	t_cmd*			cmds_conv[2];

	head = NULL;
	tail = NULL;
	while (operators /*&& !(operators->type & EXCEPTION)*/)
	{
		cmds_conv[0] = NULL;
		cmds_conv[1] = NULL;
		if (tail)
		{
			ft_dprintf(2, "2\n");
			ft_dprintf(2, "operator is [%d]\n", operators->type);
			//ft_dprintf(2, "%s\n", cmds->av[0]);
			cmds_conv[1] = cmds;
			cmds = cmds->next;
		}
		else if (cmds && (cmds_conv[0] = cmds) && (cmds = cmds->next))
		{
			ft_dprintf(2, "1\n");
			ft_dprintf(2, "%s\n", cmds->av[0]);
			cmds_conv[1] = cmds;
			cmds = cmds->next;
			head = tail;
		}
		if (!(tail = new_node(operators->type, cmds_conv, tail)))
			return (NULL);
		if (!head)
			head = tail;
		operators = operators->next;
	}
	return (head);
}

bool			cmd_is_pipe(t_bst* head)
{
	t_bst*	cp;

	cp = head;
	while (cp)
	{
		if (cp->operator & PIPE)
			return (true);
		cp = cp->next;
	}
	return (false);
}

bool			execute_bst(t_bst* head, t_bst* exec, t_term* term)
{
	t_args	args;

	if (cmd_is_pipe(head))
	{
		ft_dprintf(2, "[exec][pipe] executing...\n");
		exec_pipe_cmd(head, term, STDIN_FILENO, 0);
	}
	else
	{
		ft_dprintf(2, "[exec][cmd] executing... operator is [%d]\n", head->operator);
		if (!exec)
		{
			args.ac = head->ac[0];
			args.av = head->av[0];
		}
		else
		{
			args.ac = exec->ac[0];
			args.av = exec->av[0];
		}
		if (redir_fds(args.fds, head->av[1] ? head->av[1][0] : NULL, head->operator, STDIN_FILENO))
		{
			if (head->next && !(head->next->operator & PIPE))
				return (execute_bst(head->next, exec ? exec : head, term));
			ft_dprintf(2, "EXEC\n");
			exec_cmd(&args, term);
		}
		free_env_and_path(&args);
		return (true);
		//return (close_fds(args.fds));
	}
	return (false);
}
