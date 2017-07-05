#ifndef TERM_H
# define TERM_H
# include "libft.h"

# define MAX_PROMPT_SIZE 256
# define ASK_CURSOR_POS "\033[6n"
# ifndef TERM_C

typedef struct	s_term
{
	char			prompt[MAX_PROMPT_SIZE + 1];
	int				prompt_len;
	int				term_width;
	int				term_height;
	int				first_line_len;
	int				cursor_col;
	int				cursor_row;
//struct termios	termcaps_old;
//struct termios	termcaps_new;
	void			(*const get_term_size)(struct s_term *term);
	void			(*const get_prompt)(struct s_term *term);
	void			(*const print_prompt)(struct s_term *term, char *color);
	void			(*const get_cursor_pos)(struct s_term *term);
}				t_term;
# else
typedef struct	s_term
{
	char			prompt[MAX_PROMPT_SIZE + 1];
	int				prompt_len;
	int				term_width;
	int				term_height;
	int				first_line_len;
	int				cursor_col;
	int				cursor_row;
//	struct termios  termcaps_old;
//	struct termios  termcaps_new;
	void			(*get_term_size)(struct s_term *term);
	void			(*get_prompt)(struct s_term *term);
	void			(*print_prompt)(struct s_term *term, char *color);
	void			(*get_cursor_pos)(struct s_term *term);
}					t_term;
# endif
t_term			*init_term(void);
#endif
