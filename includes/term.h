#ifndef TERM_H
# define TERM_H
# include "libft.h"
# include "term.h"
# include "termios.h"
# include "termcap.h"
# define MAX_PROMPT_SIZE 256

typedef struct termios	t_termios;

typedef struct		s_term
{
	char			prompt[MAX_PROMPT_SIZE + 1];
	int				prompt_len;
	int				width;
	int				height;
	int				first_line_len;
	int				cursor_col;
	int				cursor_row;
	struct termios	*initial_term_settings;
	struct termios	*termcaps_term_settings;
	void			(*get_term_size)(struct s_term *term);
	void			(*get_prompt)(struct s_term *term);
	void			(*print_prompt)(struct s_term *term, char *color);
}					t_term;

t_term				*init_term(void);
#endif
