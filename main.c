#include <libft.h>

static int	val_cmp(void *a, void *b)
{
	return (*(int*)a - *(int*)b);
}

static void	val_print(void *num)
{
	ft_dprintf(1, "%d\n", *(int*)num);
}

int			main(void)
{
	int     values[] = {1, 0, 2, -1, -1};
	t_list  *lst = NULL;
	int     i;

	i = 0;
	while (i < sizeof(values) / sizeof(*values))
	{
		ft_dprintf(2, "Inserting '%d'...\n", values[i]);
		ft_lstinsert(&lst, ft_lstnew(&values[i++]), &val_cmp);
	}
	ft_lstiter(lst, &val_print);
	return (0);
}