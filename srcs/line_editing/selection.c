/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 13:36:10 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/21 14:00:49 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	switch_input_state(t_input *input)
{
	int	i;
	int	curs;

	if (input->state)
		input->state = STANDARD;
	else
		input->state = SELECTION;
	if (input->pivot == -1)
		input->pivot = input->cursor_pos;
	else
	{
		input->pivot = -1;
		curs = input->cursor_pos;
		handle_home(input);
		display_buffer(input, 0);
		i = input->buffer_len;
		while (i-- > curs)
			handle_arrow_left(input);
	}
	return (0);
}

int	skip_key(t_input *input)
{
	(void)input;
	return (0);
}

int	select_right(t_input *input)
{
	int	cursor;
	int	i;

	if (input->cursor_pos != input->buffer_len)
	{
		cursor = input->cursor_pos + 1;
		handle_home(input);
		display_buffer(input, cursor);
		i = input->buffer_len;
		while (i-- > cursor)
			handle_arrow_left(input);
	}
	return (0);
}

int	select_left(t_input *input)
{
	int	i;
	int	cursor;

	if (input->cursor_pos > 0)
	{
		cursor = input->cursor_pos - 1;
		handle_home(input);
		display_buffer(input, cursor);
		i = input->buffer_len;
		while (i-- > cursor)
			handle_arrow_left(input);
	}
	return (0);
}
