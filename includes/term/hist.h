#ifndef HIST_H
# define HIST_H

#include <term/line.h>

typedef struct		s_hist
{
	t_line	*head;
	t_line	*last;
	t_line	*curr;
	t_line	*next;
}					t_hist;

/*
**					hist.c
*/
void				hist_add(t_hist *hist, t_line *line);
void				hist_pop(t_hist *hist);
void				hist_clear(t_hist *hist);
void				hist_commit(t_hist *hist, t_line *line);

#endif