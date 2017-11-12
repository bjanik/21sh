/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_eof.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/12 14:46:47 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/12 17:11:27 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int		get_displayed_lines(t_input *input)
{
	int	remain;
	int	mod;

	dprintf(input->fd, "[%d]\n", input->buffer_len - input->term->first_line_len);
	if (input->buffer_len < input->term->first_line_len)
		return (1);
	remain = input->buffer_len - input->term->first_line_len;
	mod = remain % input->term->width;
	if (remain < input->term->width)
		return (2);
	if (mod > 0)
		return (remain / input->term->width + 1);
	return (remain / input->term->width);
}

int		handle_clear_line(t_input *input)
{
	if (input->buffer_len > 0)
	{
		handle_home(input);
		tputs(tgetstr("dl", NULL), 1, ft_putchar_termcaps);
	}
	return (0);
}

int		handle_eof(t_input *input)
{
	(void)input;
	if (input->buffer_len == 0)
	{
		ft_putendl_fd("exit", STDERR);
		exit(0);
	}
	return (0);
}
