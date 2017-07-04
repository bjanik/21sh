#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

# define INITIAL_HISTO_SIZE 512

typedef struct	s_history
{
	int	history_size;
	int	history_len;
	int	current_pos;
	char	**history;
	void	(*realloc_history)(struct s_history *history);
	void	(*append_history)(struct s_history *history, char *command);
}		t_history;

void		append_history(struct s_history *history, char *command);
t_history	*init_history(void);
	
#endif
