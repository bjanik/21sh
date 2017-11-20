/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 14:19:38 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/20 15:19:09 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	handle_history_search(t_input *input)
{
	t_dlst	*h;

	handle_home(input);
	ft_bzero(input->buffer, input->buffer_size);
	input->cursor_pos = 0;
	input->buffer_len = 0;
	tputs(tgetstr("cr", NULL), 1, ft_putchar_termcaps);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
	ft_printf("History Search> ");
	input->term->prompt_len = 16;
	wait_for_input(input, REGULAR_INPUT);
	input->buffer[--input->buffer_len] = '\0';
	h = input->history->head;
	while (h)
	{
		if (ft_strstr(h->data, input->buffer))
		{
			handle_home(input);
			tputs(tgetstr("cr", NULL), 1, ft_putchar_termcaps);
			tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
			print_prompt(input->term, BOLD_CYAN);
			input->history->current = h;
			cp_history_to_buffer(input);
			return (0);
		}
		h = h->next;
	}
	ft_printf("No matches...\n");
	ft_bzero(input->buffer, input->buffer_size);
	input->cursor_pos = 0;
	input->buffer_len = 0;
	return (0);
}
