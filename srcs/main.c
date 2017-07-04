#include "term.h"
#include "input.h"
#include "libft.h"


int main(int argc, char **argv, char **env)
{
	t_term		*term;
	t_input		*input;
	t_history	*history;
	
	term = init_term();
	history = init_history();
	input = init_input(term, history);
	check_term();
	while (42)
	{
		if (input->buffer_len == 0)
			tputs(tgetstr("sc", NULL), 1, ft_putchar_termcaps);
		ft_bzero(input->read_buffer, MAX_KEY_LENGTH);
		if (read(STDIN, input->read_buffer, MAX_KEY_LENGTH) < 1)
			exit(-1);
		input->get_key(input);
	}
	return (0);
}
