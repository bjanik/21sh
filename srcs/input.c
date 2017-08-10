/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:55:18 by bjanik            #+#    #+#             */
/*   Updated: 2017/07/05 17:44:08 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "libft.h"
#include "fcntl.h"

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

int	handle_reg_char(t_input *input, char c)
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
	if (input->term->cursor_col == input->term->width)
	{
		tputs(tgetstr("do", NULL), 1, ft_putchar_termcaps);
		input->term->cursor_col = 0;
	}
	input->buffer_len++;
	input->cursor_line_pos++;
	input->term->cursor_col++;
	return (0);
}

void	delete_char(t_input *input)
{
	if (input->cursor_line_pos == input->buffer_len)
		return ;
	tputs(tgetstr("dm", NULL), 1, ft_putchar_termcaps);
	ft_strcpy(input->buffer + input->cursor_line_pos,
				input->buffer + input->cursor_line_pos + 1);
	input->buffer_len--;
	tputs(tgetstr("dc", NULL), 1, ft_putchar_termcaps);
}

void	cp_history_to_buffer(t_input *input, int current_pos)
{
	int	command_len;

	command_len = 0;
	if (input->history->len != 0)
		command_len = ft_strlen(input->history->history[current_pos]);
	while (input->buffer_size < command_len)
		input->realloc_buffer(input);
	ft_strcpy(input->buffer, input->history->history[current_pos]);
	input->buffer_len = ft_strlen(input->buffer);
	input->cursor_line_pos = input->buffer_len;
}

void	print_buffer(t_input *input)
{
	ft_putstr_fd(input->buffer, STDIN);
}

t_keys  handle_keys[] = {
        {ARROW_UP, handle_arrow_up},
        {ARROW_DOWN, handle_arrow_down},
        {ARROW_LEFT, handle_arrow_left},
        {ARROW_RIGHT, handle_arrow_right},
        {CTRL_UP, handle_ctrl_up},
        {CTRL_DOWN, handle_ctrl_down},
        {CTRL_LEFT, handle_ctrl_left},
        {CTRL_RIGHT, handle_ctrl_right},
        {DEL, handle_del},
        {DELETE, handle_delete},
        {END, handle_end},
        {HOME, handle_home},
        {RETURN, handle_return},
};

int	get_key(t_input *input)
{
	int	i;

	i = -1;
	while (++i < NB_KEYS)
	{
		if (!ft_strcmp(handle_keys[i].key, input->read_buffer))
			return (handle_keys[i].handle_keystroke(input));
	}
	dprintf(input->fd, "%d && %d\n", input->term->width, input->term->cursor_col);
	return (ft_isprint(input->read_buffer[0])) ? handle_reg_char(input, input->read_buffer[0]): 0;
}

t_input *init_input(t_term *term, t_history *history)
{
        t_input *input;
        int     fd;

        input = (t_input*)malloc(sizeof(t_input));
        input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
        input->buffer_size = INITIAL_BUFFER_SIZE;
        input->buffer_len = 0;
        input->cursor_line_pos = 0;
        input->get_key = get_key;
        input->realloc_buffer = realloc_buffer;
        input->print_buffer = print_buffer;
        input->cp_history_to_buffer = cp_history_to_buffer;
        input->term = term;
        input->history = history;
        fd = open("/dev/pts/2", O_WRONLY | O_CREAT);
        input->fd = fd;
        return (input);
}

