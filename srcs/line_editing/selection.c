/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 13:36:10 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/17 16:13:53 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	switch_input_state(t_input *input)
{
	if (input->state)
		input->state = STANDARD;
	else
		input->state = SELECTION;
	if (input->pivot == NULL)
		input->pivot = input->buffer + input->cursor_pos;
	else
		input->pivot = NULL;
	return (0);
}

int	skip_key(t_input *input)
{
	(void)input;
	return (0);
}

int	select_right(t_input *input)
{
	if (input->cursor_pos != input->buffer_len)
	{
		tputs(tgetstr("mr", NULL), 1, ft_putchar_termcaps);
		write(STDIN, &input->buffer[input->cursor_pos], 1);
		handle_arrow_left(input);
		input->cursor_pos++;
		input->term->cursor_col++;
		handle_arrow_right(input);
	}
	return (0);
}

int	select_left(t_input *input)
{
	tputs(tgetstr("mr", NULL), 1, ft_putchar_termcaps);
	write(STDIN, &input->buffer[input->cursor_pos], 1);
	handle_arrow_right(input);
	handle_arrow_left(input);
	return (0);
}
