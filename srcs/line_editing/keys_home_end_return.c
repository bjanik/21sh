/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_home_end_return.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:16:44 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 14:25:25 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

int	handle_home(t_input *input)
{
	while (input->cursor_pos > 0)
		handle_arrow_left(input);
	return (0);
}

int	handle_end(t_input *input)
{
	while (input->cursor_pos < input->buffer_len)
		handle_arrow_right(input);
	return (0);
}

int	handle_return(t_input *input)
{
	if (input->buffer_len > 0)
	{
		handle_end(input);
		input->cursor_pos = 0;
	}
	if (input->buffer_len == input->buffer_size)
		realloc_buffer(input);
	input->buffer[input->buffer_len++] = '\n';
	return (1);
}
