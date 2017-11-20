#include "bsh.h"

void	display_buffer(t_input *input)
{
	int 	i;

	i = -1;
	while (input->buffer[++i])
	{
		write(STDOUT, &input->buffer[i], 1);
		if (input->term->cursor_col == input->term->width)
		{
			tputs(tgetstr("do", NULL), 1, ft_putchar_termcaps);
			input->term->cursor_col = 0;
		}
		input->cursor_pos++;
		input->term->cursor_col++;
	}
}
