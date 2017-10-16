#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

typedef struct	s_history
{
	int			len;
	t_dlst		*head;
	t_dlst		*end;
	t_dlst		*current;
	int			fd;
}				t_history;

t_history		*init_history(void);
void			append_history(t_history *history, char *cmd, int cmd_size);

#endif
