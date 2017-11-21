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
	if (input->cursor_pos > input->pivot)
	{
		input->buf_tmp = ft_strndup(input->buffer + input->pivot,
				input->cursor_pos - input->pivot);
	}
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
	if (input->buf_tmp)
	{
		ft_memmove((char*)input->buffer + input->cursor_pos + 1,
				(char*)input->buffer + input->pivot, ft_strlen(input->buf_tmp));
		ft_memcpy(input->buffer + input->pivot + 1, input->buf_tmp,
				ft_strlen(input->buf_tmp));
	}
	return (0);
}
