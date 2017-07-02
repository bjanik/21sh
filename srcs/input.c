/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/30 14:33:20 by bjanik            #+#    #+#             */
/*   Updated: 2017/06/30 18:29:23 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TOsh.h"

# define MAX_KEY_LENGTH 5
# define ESC 27
# define DEL 127

int	ft_my_putchar(int c)
{
	return (write(0, &c, 1));
}

int		check_term(void)
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

void		get_key(char *buff, char *command, int *command_len)
{
	if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'A')
		;
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'B')
		;
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'C')
		tputs(tgetstr("nd", NULL), 1, ft_my_putchar);
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'D')
	{
		tputs(tgetstr("le", NULL), 1, ft_my_putchar);
	}
	//else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'H')
	//else if (buff[0] == ESC && buff[1] == '[' && buff[2] == 'F')
	else if (buff[0] == '\n')
		ft_printf("\n[%s]\n", command);
	else if (ft_isprint(buff[0]))
	{
		write(0, &buff[0], 1);
		command[*command_len] = buff[0];
		(*command_len)++;
	}
	else if (buff[0] == DEL)
	{
		tputs(tgetstr("le", NULL), 1, ft_my_putchar);
		tputs(tgetstr("dc", NULL), 1, ft_my_putchar);
	}
	else if (buff[0] == ESC && buff[1] == '[' && buff[2] == '3' && buff[3] == '~')
	{
		tputs(tgetstr("dc", NULL), 1, ft_my_putchar);
	}
}

int main(int argc, char **argv, char **env)
{
	char	buff[MAX_KEY_LENGTH];
	int		command_len;
	command_len = 0;

	command = (char*)ft_malloc(513);
	ft_bzero(command, 513);
	check_term();
	while (42)
	{
		ft_bzero(buff, MAX_KEY_LENGTH);
		if (read(STDIN, buff, MAX_KEY_LENGTH) < 1)
			exit(-1);
		get_key(buff, command, &command_len);
	}
}
