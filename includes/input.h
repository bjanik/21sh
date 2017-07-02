#ifndef INPUT_H
# define INPUT_H

typedef struct	s_input
{
	char		*buffer;
	int			buffer_len;
	int			buffer_size;
	int			cursor_line_pos;
	void		(*insert_char)(struct s_input *input, char c);
	void		(*delete_char)(struct s_input *input);
	void		(*realloc_buffer)(struct s_input *input);
	void		(*move_cursor_right)(struct s_input *input);
	void		(*move_cursor_left)(struct s_input *input);
}				t_input;

#endif
