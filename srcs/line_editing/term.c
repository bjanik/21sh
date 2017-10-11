/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:55:29 by bjanik            #+#    #+#             */
/*   Updated: 2017/07/05 16:18:21 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define TERM_C
#include "term.h"

static void	get_term_size(t_term *term)
{
	struct winsize	winsize;

	ioctl(STDIN, TIOCGWINSZ, &winsize);
	term->width = winsize.ws_col;
	term->height = winsize.ws_row;
}

static void	get_prompt(t_term *term)
{
	char	*pwd;

	if ((pwd = getcwd(NULL, MAX_PROMPT_SIZE)) == NULL)
	{
		ft_putendl_fd("getcwd failed", STDERR);
		exit(-1);
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

static void	print_prompt(t_term *term, char *color)
{
	term->get_prompt(term);
	ft_printf("%s%s%s ", color, term->prompt, RESET);
}

t_term		*init_term(void)
{
	t_term	*term;

	term = (t_term*)malloc(sizeof(t_term));
	term->prompt_len = 0;
	term->cursor_row = 0;
	term->cursor_col = 0;
	term->get_term_size = get_term_size;
	term->get_prompt = get_prompt;
	term->print_prompt = print_prompt;
	term->get_term_size(term);
	term->get_prompt(term);
	return (term);
}
