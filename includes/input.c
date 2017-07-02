#include "input.h"
#include "libft.h"
#include <term.h>
#include <termios.h>
#include <termcap.h>

# define MAX_KEY_LENGTH 5
# define ESC 27
# define DEL 127
# define INITIAL_BUFFER_SIZE 512

int	ft_my_putchar(int c)
{
	return (write(0, &c, 1));
}

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
	tputs(tgetstr("im", NULL), 1, ft_my_putchar);
	write(STDIN, &c, 1);
	tputs(tgetstr("ei", NULL), 1, ft_my_putchar);
	input->buffer_len++;
	input->cursor_line_pos++;
}

static void	delete_char(t_input *input)
{
	if (input->cursor_line_pos == input->buffer_len)
		return ;
	tputs(tgetstr("dm", NULL), 1, ft_my_putchar);
	ft_strcpy(input->buffer + input->cursor_line_pos,
				input->buffer + input->cursor_line_pos + 1);
	input->buffer_len--;
	tputs(tgetstr("dc", NULL), 1, ft_my_putchar);
}

static void	move_cursor_left(t_input *input)
{
	if (input->cursor_line_pos > 0)
	{
		tputs(tgetstr("le", NULL), 1, ft_my_putchar);
		input->cursor_line_pos--;
	}
}

static void	move_cursor_right(t_input *input)
{
	if (input->cursor_line_pos < input->buffer_len)
	{
		tputs(tgetstr("nd", NULL), 1, ft_my_putchar);
		input->cursor_line_pos++;
	}
}

void		get_key(t_input *input, char *buff)
{
	if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'A')
		;
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'B')
		;
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'C')
		input->move_cursor_right(input);
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'D')
		input->move_cursor_left(input);
	//else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'H')
	//else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'F')
	else if (buff[0] == '\n')
		ft_printf("\n[%s]\n", input->buffer);
	else if (buff[0] == DEL)
	{
		input->move_cursor_left(input);
		input->delete_char(input);
	}
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == '3' && buff[3] == '~')
		input->delete_char(input);
	else if (ft_isprint(buff[0]))
	{
		input->insert_char(input, buff[0]);
	}
}

t_input	*init_input(void)
{
	t_input *input;

	input = (t_input*)malloc(sizeof(t_input));
	input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
	input->buffer_size = INITIAL_BUFFER_SIZE;
	input->buffer_len = 0;
	input->cursor_line_pos = 0;
	input->insert_char = insert_char;
	input->delete_char = delete_char;
	input->realloc_buffer = realloc_buffer;
	input->move_cursor_left = move_cursor_left;
	input->move_cursor_right = move_cursor_right;
	return (input);
}

int main(int argc, char **argv, char **env)
{
	char	buff[MAX_KEY_LENGTH];
	t_input	*input;

	input = init_input();
	check_term();
	while (42)
	{
		ft_bzero(buff, MAX_KEY_LENGTH);
		if (read(STDIN, buff, MAX_KEY_LENGTH) < 1)
			exit(-1);
		get_key(input, buff);
	}
}
