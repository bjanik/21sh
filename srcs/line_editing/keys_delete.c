#include "input.h"

static void	delete_char(t_input *input)
{
	if (input->cursor_pos == input->buffer_len)
		return ;
	tputs(tgetstr("dm", NULL), 1, ft_putchar_termcaps);
	ft_strcpy(input->buffer + input->cursor_pos,
				input->buffer + input->cursor_pos + 1);
	input->buffer_len--;
	tputs(tgetstr("dc", NULL), 1, ft_putchar_termcaps);
	update_visual_buffer(input);
}

int     handle_backspace(t_input *input)
{
	if (input->cursor_pos > 0)
        {
		handle_arrow_left(input);
        	delete_char(input);
	}
        return (0);
}

int     handle_delete(t_input *input)
{
        delete_char(input);
        return (0);
}

