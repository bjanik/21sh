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
	term->term_width = winsize.ws_col;
	term->term_height = winsize.ws_row;
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
	term->prompt_len = ft_strlen(term->prompt);
	term->first_line_len = term->term_width - (term->prompt_len + 1);
}

static void	print_prompt(t_term *term, char *color)
{
	ft_printf("%s%s%s ", color, term->prompt, RESET);
}

static void	get_cursor_pos(t_term *term)
{
	char	buf[9];
	char	**split;

	ft_bzero(buf, 7);
	write(STDOUT, ASK_CURSOR_POS, 5);
	read(STDIN, buf, 8);
	split = ft_strsplit(buf, ';');
	term->cursor_row = ft_atoi(split[0] + 2);
	term->cursor_col = (!split[1]) ? 0 : ft_atoi(split[1]);
	ft_free_string_tab(&split);
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
	term->get_cursor_pos = get_cursor_pos;
	return (term);
}
