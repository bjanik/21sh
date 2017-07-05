#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

# define INITIAL_HISTO_SIZE 512
# ifndef HISTORY_C
typedef struct	s_history
{
	int			history_size;
	int			history_len;
	int			current_pos;
	char		**history;
	void		(*realloc_history)(struct s_history *history);
	void		(*append_history)(struct s_history *history, char *command);
}				t_history;
# else
typedef struct	s_history
{
	int			history_size;
	int			history_len;
	int			current_pos;
	char		**history;
	void		(*realloc_history)(struct s_history *history);
	void		(*append_history)(struct s_history *history, char *command);
}				t_history;
# endif
void		append_history(struct s_history *history, char *command);
t_history	*init_history(void);

#endif
