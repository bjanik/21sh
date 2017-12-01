/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 16:48:42 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/01 11:47:24 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

t_input		*init_input(t_term *term, t_history *history)
{
	t_input	*input;

	if (!(input = (t_input*)malloc(sizeof(t_input))))
		ft_error_msg("Malloc failed");
	input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
	input->buf_tmp = NULL;
	input->buf_copy = NULL;
	input->buffer_size = INITIAL_BUFFER_SIZE;
	input->buffer_len = 0;
	input->cursor_pos = 0;
	input->term = term;
	input->history = history;
	input->fd = open("/dev/ttys002", O_WRONLY | O_CREAT);
	input->state = STANDARD;
	input->pivot = -1;
	return (input);
}
