/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:55:29 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/27 17:16:09 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int				restore_custom_attr(t_term *term)
{
	if (tcsetattr(STDIN, TCSANOW, &term->custom_attr) == -1)
		ft_error_msg("Unable to set terminal");
	return (0);
}

int					init_termcaps(t_term *term)
{
	char			*termtype;

	if (!isatty(STDIN))
		exit(EXIT_FAILURE);
	if (!(termtype = getenv("TERM")))
		ft_error_msg("Missing $TERM variable");
	if (!tgetent(NULL, termtype))
		exit(EXIT_FAILURE);
	if (tcgetattr(STDIN, &term->initial_attr) == -1)
		exit(EXIT_FAILURE);
	if (tcgetattr(STDIN, &term->custom_attr) == -1)
		exit(EXIT_FAILURE);
	term->custom_attr.c_lflag &= ~(ICANON | ECHO);
	term->custom_attr.c_cc[VMIN] = 1;
	term->custom_attr.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN, TCSANOW, &term->custom_attr) == -1)
		ft_error_msg("Unable to set terminal");
	return (0);
}

int				restore_initial_attr(t_term *term)
{
	if (tcsetattr(STDIN, TCSANOW, &term->initial_attr) == -1)
		ft_error_msg("Unable to set terminal");
	return (0);
}

void				get_term_size(t_term *term)
{
	struct winsize	winsize;

	ioctl(STDIN, TIOCGWINSZ, &winsize);
	term->width = winsize.ws_col;
	term->height = winsize.ws_row;
}

void				get_prompt(t_term *term)
{
	char			*pwd;

	if ((pwd = getcwd(NULL, MAX_PROMPT_SIZE)) == NULL)
	{
		ft_putendl_fd("getcwd failed", STDERR);
		exit(EXIT_FAILURE);
	}
	ft_bzero(term->prompt, MAX_PROMPT_SIZE);
	if (pwd[0] == '/' && !pwd[1])
		term->prompt[0] = *pwd;
	else
		ft_strcpy(term->prompt, ft_strrchr(pwd, '/') + 1);
	free(pwd);
	term->prompt_len = ft_strlen(term->prompt) + 1;
	term->cursor_col = term->prompt_len + 1;
	term->first_line_len = term->width - term->prompt_len;
}

void				print_prompt(t_term *term, char *color)
{
	get_prompt(term);
	ft_printf("%s%s%s ", color, term->prompt, RESET);
}

t_term				*init_term(void)
{
	t_term			*term;

	if (!(term = (t_term*)malloc(sizeof(t_term))))
		exit(EXIT_FAILURE);
	term->prompt_len = 0;
	term->cursor_row = 0;
	term->cursor_col = 0;
	get_term_size(term);
	get_prompt(term);
	return (term);
}
