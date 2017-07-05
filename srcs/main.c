/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:55:58 by bjanik            #+#    #+#             */
/*   Updated: 2017/07/05 17:13:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term.h"
#include "termios.h"
#include "termcap.h"

#include "input.h"
#include "libft.h"


int		init_termcaps(void)
{
	char			*termtype;
	struct termios	term;

	if (!isatty(STDIN))
		exit(-1);
	if (!(termtype = getenv("TERM")))
		ft_error_msg("Missing $TERM variable");
	if (!tgetent(NULL, termtype))
		exit(-1);
	if (tcgetattr(STDIN, &term) == -1)
		exit(-1);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN, TCSADRAIN, &term) == -1)
		ft_error_msg("Unable to set terminal");
	return (0);
}

int main(int argc, char **argv, char **env)
{
	t_term		*term;
	t_input		*input;
	t_history	*history;

	term = init_term();
	history = init_history();
	input = init_input(term, history);
	init_termcaps();
	while (42)
	{
		term->print_prompt(term, BOLD_CYAN);
		while (42)
		{
			if (input->buffer_len == 0)
				tputs(tgetstr("sc", NULL), 1, ft_putchar_termcaps);
			ft_bzero(input->read_buffer, MAX_KEY_LENGTH);
			if (read(STDIN, input->read_buffer, MAX_KEY_LENGTH) < 1)
				exit(-1);
			if (input->get_key(input))
			{
				write(STDOUT, "\n", 1);
				break ;
			}
		}
	}
	return (0);
}
