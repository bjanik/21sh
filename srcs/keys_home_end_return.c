#include "input.h"

int     handle_home(t_input *input)
{
        while (input->cursor_line_pos > 0)
                handle_arrow_left(input);
        return (0);
}

int     handle_end(t_input *input)
{
        while (input->cursor_line_pos < input->buffer_len)
                handle_arrow_right(input);
        return (0);
}

int     handle_return(t_input *input)
{
        input->buffer[input->buffer_len] = '\n';
        append_history(input->history, input->buffer);
        handle_end(input);
        ft_bzero(input->buffer, input->buffer_size);
        input->cursor_line_pos = 0;
        input->history->current_pos = input->history->len;
        input->buffer_len = 0;
        return (1);
}

