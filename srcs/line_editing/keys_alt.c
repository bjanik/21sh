#include "input.h"

int	handle_alt_great(t_input *input)
{
	input->history->current = input->history->head;
	cp_history_to_buffer(input);
	return (0);
}

int	handle_alt_less(t_input *input)
{
	input->history->current = input->history->end;
	cp_history_to_buffer(input);
	return (0);
}
