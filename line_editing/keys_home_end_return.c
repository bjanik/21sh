#include "input.h"

int     handle_home(t_input *input)
{
        while (input->cursor_pos > 0)
                handle_arrow_left(input);
        return (0);
}

int     handle_end(t_input *input)
{
        while (input->cursor_pos < input->buffer_len)
                handle_arrow_right(input);
        return (0);
}

int     handle_return(t_input *input)
{
	if (input->buffer_len > 0)
        {
		append_history(input->history, input->buffer, input->buffer_len + 1);
       		input->buffer[input->buffer_len] = '\n';
	       	handle_end(input);
       		input->cursor_pos = 0;
	}
        return (1);
}

