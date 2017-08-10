#include "input.h"

int     handle_arrow_left(t_input *input)
{
        if (input->cursor_line_pos > 0)
        {
                if (input->term->cursor_col == 1)
                {
                        tputs(tgetstr("up", NULL), 1, ft_putchar_termcaps);
                        tputs(tgoto(tgetstr("RI", NULL), 0, input->term->width), 1, ft_putchar_termcaps);
                        input->term->cursor_col = input->term->width;
                }
                else
                {
                        tputs(tgetstr("le", NULL), 1, ft_putchar_termcaps);
                        input->term->cursor_col--;
                }
                input->cursor_line_pos--;
        }
        return (0);
}

int     handle_arrow_right(t_input *input)
{
        if (input->cursor_line_pos < input->buffer_len)
        {
                if (input->term->cursor_col == input->term->width)
                {
                        tputs(tgetstr("do", NULL), 1, ft_putchar_termcaps);
                        input->term->cursor_col = 1;
                }
                else
                {
                        tputs(tgetstr("nd", NULL), 1, ft_putchar_termcaps);
                        input->term->cursor_col++;
                }
                input->cursor_line_pos++;
        }
        return (0);
}

int     handle_arrow_up(t_input *input)
{
        return (0);
}

int     handle_arrow_down(t_input *input)
{
        return (0);
}

