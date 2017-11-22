/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_cut_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/21 13:32:04 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/21 15:38:07 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	copy_selection(t_input *input)
{
	if (input->pivot < 0)
		return (0);
	ft_strdel(&input->buf_tmp);
	if (input->cursor_pos > input->pivot)
	{
		input->buf_tmp = ft_strndup(input->buffer + input->pivot,
				input->cursor_pos - input->pivot);
	}
	else if (input->cursor_pos < input->pivot)
	{
		input->buf_tmp = ft_strndup(input->buffer + input->cursor_pos,
				input->pivot - input->cursor_pos);
	}
	dprintf(input->fd,"[%s]\n", input->buf_tmp);
	return (0);
}

int	cut_selection(t_input *input)
{
	int	i;
	int	cursor;

	cursor = input->cursor_pos;
	if (input->cursor_pos > input->pivot)
		ft_strcpy(input->buffer + input->pivot,
				input->buffer + input->cursor_pos);
	input->buffer_len = ft_strlen(input->buffer);
	tputs(tgetstr("me", NULL), 1, ft_putchar_termcaps);
	input->pivot = -1;
	input->state = STANDARD;
	handle_home(input);
	tputs(tgetstr("ce", NULL), 1, ft_putchar_termcaps);
	display_buffer(input, 0);
	i = input->buffer_len;
	while (i-- > cursor)
		handle_arrow_left(input);
	return (0);
}

int	paste_selection(t_input *input)
{
	int	len;

	if (input->buf_tmp)
	{
		len = ft_strlen(input->buf_tmp);
		if (input->buffer_len + len > input->buffer_size)
			realloc_buffer(input);
		ft_memmove((char*)input->buffer + input->cursor_pos + len,
				(char*)input->buffer + input->cursor_pos, len);
		ft_memcpy(input->buffer + input->cursor_pos, input->buf_tmp,
				 len);
		input->buffer_len += len;
		display_line(input, input->cursor_pos + len);
		dprintf(input->fd, "[%s] && len = %d && cursor_pos = %d\n", input->buffer, input->buffer_len, input->cursor_pos);
	}
	return (0);
}
