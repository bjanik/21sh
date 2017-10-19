/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:55:18 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 17:23:47 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int			cursor_on_last_line(t_input *input)
{
	int		i;

	i = 0;
	while (input->cursor_pos + i < input->buffer_len &&
		input->term->cursor_col + i < input->term->width)
		i++;
	if (input->cursor_pos + i == input->buffer_len)
		return (1);
	return (0);
}

void		update_visual_buffer(t_input *input)
{
	if (!cursor_on_last_line(input))
	{
		tputs(tgetstr("sc", NULL), 1, ft_putchar_termcaps);
		tputs(tgetstr("nw", NULL), 1, ft_putchar_termcaps);
		tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
		tputs(tgetstr("rc", NULL), 1, ft_putchar_termcaps);
		tputs(tgetstr("sc", NULL), 1, ft_putchar_termcaps);
		ft_putstr_fd(input->buffer + input->cursor_pos, STDIN);
		tputs(tgetstr("rc", NULL), 1, ft_putchar_termcaps);
	}
}

void		realloc_buffer(t_input *input)
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

int		handle_reg_char(t_input *input, char c)
{
	if (input->buffer_len == input->buffer_size)
		realloc_buffer(input);
	if (input->cursor_pos == input->buffer_len)
		input->buffer[input->buffer_len] = c;
	else
	{
		ft_memmove((char*)input->buffer + input->cursor_pos + 1,
		(char *)input->buffer + input->cursor_pos,
		ft_strlen(input->buffer + input->cursor_pos));
		input->buffer[input->cursor_pos] = c;
	}
	tputs(tgetstr("im", NULL), 1, ft_putchar_termcaps);
	write(STDIN, &c, 1);
	tputs(tgetstr("ei", NULL), 1, ft_putchar_termcaps);
	if (input->term->cursor_col == input->term->width)
	{
		tputs(tgetstr("do", NULL), 1, ft_putchar_termcaps);
		input->term->cursor_col = 0;
	}
	input->cursor_pos++;
	input->buffer_len++;
	update_visual_buffer(input);
	input->term->cursor_col++;
	return (0);
}

void		cp_history_to_buffer(t_input *input)
{
	int		i;
	char	*str;

	i = -1;
	str = input->history->current->data;
	ft_bzero(input->buffer, input->buffer_size);
	input->buffer_len = 0;
	handle_home(input);
	//ft_printf("IN HIST BUFFER\n");
	while (str[++i])
		handle_reg_char(input, str[i]);
	tputs(tgetstr("sc", NULL), 1, ft_putchar_termcaps);
	tputs(tgetstr("nw", NULL), 1, ft_putchar_termcaps);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
	tputs(tgetstr("rc", NULL), 1, ft_putchar_termcaps);
}
