#include "input.h"

t_keys  handle_keys[] = {
        {ARROW_UP, handle_arrow_up},
        {ARROW_DOWN, handle_arrow_down},
        {ARROW_LEFT, handle_arrow_left},
        {ARROW_RIGHT, handle_arrow_right},
        {CTRL_UP, handle_ctrl_up},
        {CTRL_DOWN, handle_ctrl_down},
        {CTRL_LEFT, handle_ctrl_left},
        {CTRL_RIGHT, handle_ctrl_right},
        {ALT_GREAT, handle_alt_great},
        {ALT_LESS, handle_alt_less},
        {BACKSPACE, handle_backspace},
        {DELETE, handle_delete},
        {END, handle_end},
        {HOME, handle_home},
        {RETURN, handle_return},
	{CLEAR_SCREEN, handle_clear_screen},
	{NULL, NULL},
};

int     get_key(t_input *input)
{
        int     i;

        i = -1;
        while (handle_keys[++i].key)
        {
                if (!ft_strcmp(handle_keys[i].key, input->read_buffer))
                        return (handle_keys[i].handle_keystroke(input));
        }
        return (ft_isprint(input->read_buffer[0])) ?
                 handle_reg_char(input, input->read_buffer[0]): 0;
}