#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

typedef struct		s_history
{
	int		len;
	t_dlst		*head;
	t_dlst		*end;
	t_dlst		*current;
	void		(*append_history)(struct s_history *history, char *cmd, int cmd_size);
}			t_history;


void			append_history(t_history *history, char *cmd, int cmd_size);
t_history		*init_history(void);

#endif
