/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 13:38:26 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/17 12:02:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void	set_signals(void)
{
	signal(SIGWINCH, winsize_change);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, sigint_handler);
}

void	winsize_change(int signum)
{
	t_input	*input;

	(void)signum;
	input = get_bsh()->input;
	get_term_size(input->term);
	handle_clear_screen(input);
}

void	sigint_handler(int signum)
{
	t_bsh	*bsh;

	(void)signum;
	bsh = get_bsh();
	write(STDOUT, "\n", 1);
	print_prompt(bsh->term, BOLD_CYAN);
	ft_bzero(bsh->input->buffer, bsh->input->buffer_size);
	bsh->input->buffer_len = 0;
	bsh->input->cursor_pos = 0;

}
