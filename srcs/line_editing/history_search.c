/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_search.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/20 14:19:38 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/03 18:49:32 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	handle_history_search(t_input *input)
{
	t_dlst	*h;

	handle_home(input);
	reset_buffer(input);
	tputs(tgetstr("cr", NULL), 1, ft_putchar_termcaps);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
	ft_printf("History Search> ");
	input->term->prompt_len = 16;
	input->term->cursor_col = 17;
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
	print_prompt(input->term, BOLD_CYAN);
	reset_buffer(input);
	return (0);
}
