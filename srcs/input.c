#include "input.h"
#include "libft.h"
#include <term.h>
#include <termios.h>
#include <termcap.h>


int		check_term(void)
{
	char			*termtype;
	struct termios	term;

	if (!isatty(STDIN))
		exit(-1);
	if (!(termtype = getenv("TERM")))
		ft_error_msg("Missing $TERM variable");
	if (!tgetent(NULL, termtype))
		exit(-1);
	if (tcgetattr(STDIN, &term) == -1)
		exit(-1);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN, TCSADRAIN, &term) == -1)
		ft_error_msg("Unable to set terminal");
	return (0);
}

static void	realloc_buffer(t_input *input)
{
	char	*tmp;

	tmp = input->buffer;
	input->buffer = (char *)ft_malloc((input->buffer_size * 2 + 1)
			* sizeof(char));
	input->buffer_size *= 2;
	ft_bzero(input->buffer, input->buffer_size + 1);
	ft_strcpy(input->buffer, tmp);
	free(tmp);
}

static void	insert_char(t_input *input, char c)
{
	if (input->buffer_len == input->buffer_size)
		input->realloc_buffer(input);
	if (input->cursor_line_pos == input->buffer_len)
		input->buffer[input->buffer_len] = c;
	else
	{
		ft_memmove((char*)input->buffer + input->cursor_line_pos + 1,
				(char *)input->buffer + input->cursor_line_pos,
				ft_strlen(input->buffer + input->cursor_line_pos + 1));
		input->buffer[input->cursor_line_pos] = c;
	}
	tputs(tgetstr("im", NULL), 1, ft_putchar_termcaps);
	write(STDIN, &c, 1);
	tputs(tgetstr("ei", NULL), 1, ft_putchar_termcaps);
	input->buffer_len++;
	input->cursor_line_pos++;
}

static void	delete_char(t_input *input)
{
	if (input->cursor_line_pos == input->buffer_len)
		return ;
	tputs(tgetstr("dm", NULL), 1, ft_putchar_termcaps);
	ft_strcpy(input->buffer + input->cursor_line_pos,
				input->buffer + input->cursor_line_pos + 1);
	input->buffer_len--;
	tputs(tgetstr("dc", NULL), 1, ft_putchar_termcaps);
}

static void	move_cursor_left(t_input *input)
{
	if (input->cursor_line_pos > 0)
	{
		tputs(tgetstr("le", NULL), 1, ft_putchar_termcaps);
		input->cursor_line_pos--;
	}
}

static void	move_cursor_right(t_input *input)
{
	if (input->cursor_line_pos < input->buffer_len)
	{
		tputs(tgetstr("nd", NULL), 1, ft_putchar_termcaps);
		input->cursor_line_pos++;
	}
}

static void	get_key(t_input *input)
{
	if (!ft_strcmp(input->read_buffer, ARROW_UP))
		input->term->get_cursor_pos(input->term);
	else if (!ft_strcmp(input->read_buffer, ARROW_DOWN))
		;
	else if (!ft_strcmp(input->read_buffer, ARROW_RIGHT))
		input->move_cursor_right(input);
	else if (!ft_strcmp(input->read_buffer, ARROW_LEFT))
		input->move_cursor_left(input);
	else if (!ft_strcmp(input->read_buffer, HOME))
	{
		tputs(tgetstr("rc", NULL), 1, ft_putchar_termcaps);
		input->cursor_line_pos = 0;	
	}
	else if (!ft_strcmp(input->read_buffer, END))
		;
	else if (input->read_buffer[0] == '\n' && input->buffer_len > 0)
	{
		input->history->append_history(input->history, input->buffer);
		ft_printf("\n[%s]\n", input->buffer);
		ft_bzero(input->buffer, input->buffer_size);
	}
	else if (input->read_buffer[0] == DEL)
	{
		input->move_cursor_left(input);
		input->delete_char(input);
	}
	else if (input->read_buffer[0] == 27 && input->read_buffer[1] == '[' &&
		input->read_buffer[2] == '3' && input->read_buffer[3] == '~')
		input->delete_char(input);
	else if (ft_isprint(input->read_buffer[0]))
		input->insert_char(input, input->read_buffer[0]);
}

void	cp_history_to_buffer(t_input *input, int current_pos)
{
	int	command_len;

	command_len = ft_strlen(input->history->history[current_pos]);
	while (input->buffer_size < command_len)
		input->realloc_buffer(input);
	ft_strcpy(input->buffer, input->history->history[current_pos]);
	input->buffer_len = ft_strlen(input->buffer);
	input->buffer_size = input->buffer_len + 1;
	input->cursor_line_pos = input->buffer_len;
}

void	print_buffer(t_input *input)
{
	
	
}

t_input	*init_input(t_term *term, t_history *history)
{
	t_input *input;

	input = (t_input*)malloc(sizeof(t_input));
	input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
	input->buffer_size = INITIAL_BUFFER_SIZE;
	input->buffer_len = 0;
	input->cursor_line_pos = 0;
	input->get_key = get_key;
	input->insert_char = insert_char;
	input->delete_char = delete_char;
	input->realloc_buffer = realloc_buffer;
	input->move_cursor_left = move_cursor_left;
	input->move_cursor_right = move_cursor_right;
	input->term = term;
	input->history = history;
	return (input);
}
