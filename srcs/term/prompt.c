/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:17:41 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/12 16:39:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		get_prompt(t_term *term)
{
	char	*pwd;

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

void		display_prompt(t_input *input)
{
	input->term->prompt_len = 2;
	ft_bzero(input->buffer, input->buffer_len);
	ft_strcpy(input->term->prompt, "> ");
	write(STDOUT, "> ", 2);
	input->cursor_pos = 0;
	input->buffer_len = 0;
	input->term->cursor_col = 3;
}

void		print_prompt(t_term *term, char *color)
{
	get_prompt(term);
	ft_printf("%s%s%s ", color, term->prompt, RESET);
}
