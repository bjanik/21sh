/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_eof.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 14:46:47 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/03 19:15:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

/*int		get_displayed_lines(t_input *input)
{
	int	remain;
	int	mod;

	if (input->buffer_len < input->term->first_line_len)
		return (1);
	remain = input->buffer_len - input->term->first_line_len;
	mod = remain % input->term->width;
	if (remain < input->term->width)
		return (2);
	if (mod > 0)
		return (remain / input->term->width + 1);
	return (remain / input->term->width);
}*/

int		handle_clear_line(t_input *input)
{
	if (input->buffer_len > 0)
	{
		handle_home(input);
		tputs(tgetstr("cd", NULL), 1, ft_putchar_termcaps);
		ft_bzero(input->buffer, input->buffer_size);
		input->buffer_len = 0;
		input->history->current = NULL;
	}
	return (0);
}

int		handle_eof(t_input *input)
{
	if (input->buffer_len == 0 && input->type == REGULAR_INPUT)
	{
		ft_putendl_fd("exit", STDERR);
		save_history_to_hist_file();
		restore_initial_attr(get_bsh()->term);
		exit(0);
	}
	if (input->buffer_len == 0 && input->type == HEREDOC_INPUT)
		return (STOP_HEREDOC);
	if (input->buffer_len == 0 && input->type == UNCLOSED_QUOTES)
		return (UNEXPECTED_EOF);
	return (0);
}
