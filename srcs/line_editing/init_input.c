#include "input.h"

t_input *init_input(t_term *term, t_history *history)
{
	t_input *input;
	int	fd;

	if (!(input = (t_input*)malloc(sizeof(t_input))))
		exit(EXIT_FAILURE);
	input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
	input->buf_tmp = NULL;
	input->buffer_size = INITIAL_BUFFER_SIZE;
	input->buffer_len = 0;
	input->cursor_pos = 0;
	input->term = term;
	input->history = history;
	//fd = open("/dev/ttys000", O_WRONLY | O_CREAT);
	input->fd = fd;
	return (input);
}
