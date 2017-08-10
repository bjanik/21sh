#include "input.h"

int     handle_del(t_input *input)
{
        handle_arrow_left(input);
        delete_char(input);
        return (0);
}

int     handle_delete(t_input *input)
{
        delete_char(input);
        return (0);
}

