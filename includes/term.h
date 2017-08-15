#ifndef TERM_H
# define TERM_H
# include "libft.h"

# define MAX_PROMPT_SIZE 256

typedef struct	s_term
{
	char			prompt[MAX_PROMPT_SIZE + 1];
	int				prompt_len;
	int				width;
	int				height;
	int				first_line_len;
	int				cursor_col;
	int				cursor_row;
	//struct termios	termcaps_old;
	//struct termios	termcaps_new;
	void			(*get_term_size)(struct s_term *term);
	void			(*get_prompt)(struct s_term *term);
	void			(*print_prompt)(struct s_term *term, char *color);
}				t_term;
t_term			*init_term(void);
#endif
