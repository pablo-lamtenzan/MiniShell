#include <signals.h>

static int	sig_cmp(void *a, void *b)
{
	if (a == -1)
		return (1);
	return (a - b);
}

t_list	*load_signals(void)
{
	t_list	*sig_list;
	t_list	*sig;
	int		i;

	sig_list = NULL;
	i = 0;
	while (i < sizeof(g_signals) / sizeof(*g_signals))
	{
		if (!(sig = ft_lstnew(&g_signals[i])))
		{
			ft_lstclear(&sig_list, &free);
			return (NULL);
		}
		ft_lstinsert(&sig_list, sig, &sig_cmp);
		i++;
	}
	return (sig_list);
}

t_signal	*signal_get_value(t_list *signals, const char *name)
{
	t_list	*curr;

	curr = signals;
	while (curr && ft_strcmp(((t_signal*)curr->content)->name, name))
		curr = curr->next;
	return (curr);
}

t_signal	*signal_get_name(t_list *signals, int value)
{
	t_list	*curr;

	curr = signals;
	while (curr && ((t_signal*)curr->content)->value == value)
		curr = curr->next;
	return (curr);
}
