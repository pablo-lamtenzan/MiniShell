#include <execution/execution.h>
#include <term/term.h>

int	b_history(t_exec *args)
{
	t_line	*curr;
	size_t	i;

	curr = g_term.caps.hist.head;
	i = 1;
	(void)args;
	while (curr && curr != g_term.caps.hist.next)
	{
		ft_dprintf(1, "%lu  %s\n", i++, curr->data);
		curr = curr->next;
	}
	return (0);
}
