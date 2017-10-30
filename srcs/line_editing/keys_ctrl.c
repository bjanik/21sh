# include "input.h"

int		handle_ctrl_down(t_input *input)
{
	if (input->cursor_pos + input->term->width < input->buffer_len)
	{
		tputs(tgetstr("do", NULL), 1, ft_putchar_termcaps);
		tputs(tgoto(tgetstr("RI", NULL), 0, input->term->cursor_col - 1),
		1, ft_putchar_termcaps);
		input->cursor_pos += input->term->width;
	}
	else
		handle_end(input);
	return (0);
}

int		handle_ctrl_up(t_input *input)
{
	int	nb;

	nb = input->term->prompt_len - input->term->cursor_col + 1;
	if (input->cursor_pos >= input->term->first_line_len)
	{
		tputs(tgetstr("up", NULL), 1, ft_putchar_termcaps);
		if (input->term->cursor_col <= input->term->prompt_len &&
			input->cursor_pos < input->term->first_line_len +
			input->term->width)
		{
			tputs(tgoto(tgetstr("RI", NULL), 0, nb), 1, ft_putchar_termcaps);
			input->cursor_pos = 0;
			input->term->cursor_col = input->term->prompt_len + 1;
		}
		else
			input->cursor_pos -= input->term->width;
	}
	return (0);
}

int		handle_ctrl_right(t_input *input)
{
	if (input->cursor_pos != input->buffer_len)
	{
		while (ft_isprint(input->buffer[input->cursor_pos]))
		{
				handle_arrow_right(input);
				if (input->buffer[input->cursor_pos] == ' ' &&
				input->buffer[input->cursor_pos - 1] != ' ')
	 			break;
		}
	}
	return (0);
}

int		handle_ctrl_left(t_input *input)
{
	if (input->cursor_pos)
	{
		while (ft_isprint(input->buffer[input->cursor_pos]) ||
		!input->buffer[input->cursor_pos])
		{
			handle_arrow_left(input);
			if ((input->buffer[input->cursor_pos] != ' ' &&
				input->buffer[input->cursor_pos - 1] == ' ') ||
				!input->cursor_pos)
					break;
			}
	}
	return (0);
}

int		handle_clear_screen(t_input *input)
{
	tputs(tgetstr("cl", NULL), 1, ft_putchar_termcaps);
	print_prompt(input->term, BOLD_CYAN);
	ft_bzero(input->buffer, input->buffer_len);
	input->buffer_len = 0;
	input->cursor_pos = 0;
	return (0);
}
