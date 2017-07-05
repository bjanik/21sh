#ifndef INPUT_H
# define INPUT_H
# include "libft.h"
# include "term.h"
# include "history.h"

# define MAX_KEY_LENGTH 4
# define MAX_PROMPT_SIZE 256
# define INITIAL_BUFFER_SIZE 512

# define ARROW_RIGHT "\x1B[C"
# define ARROW_LEFT "\x1B[D"
# define ARROW_UP "\x1B[A"
# define ARROW_DOWN "\x1B[B"
# define DELETE "\x1B[3~"
# define DEL 127
# define HOME "\x1B[H"
# define END "\x1B[F"

typedef struct	s_input
{
	char		*buffer;
	int			buffer_len;
	int			buffer_size;
	int			cursor_line_pos;
	char		read_buffer[MAX_KEY_LENGTH + 1];
	int			(*get_key)(struct s_input *input);
	void		(*insert_char)(struct s_input *input, char c);
	void		(*delete_char)(struct s_input *input);
	void		(*realloc_buffer)(struct s_input *input);
	void		(*move_cursor_right)(struct s_input *input);
	void		(*move_cursor_left)(struct s_input *input);
	void		(*print_buffer)(struct s_input *input);
	void		(*cp_history_to_buffer)(struct s_input *input,
				 int histo_current_pos);
	t_term		*term;
	t_history	*history;
}				t_input;
t_input	*init_input(t_term *term, t_history *history);
#endif
