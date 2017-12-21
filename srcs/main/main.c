/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/21 16:18:38 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static void	connect_tokens(t_bsh *bsh)
{
	bsh->tokens[0] = bsh->lexer->token_list[0];
	bsh->tokens[1] = bsh->lexer->token_list[1];
	bsh->lexer->token_list[0] = NULL;
	bsh->lexer->token_list[1] = NULL;
}

static void	start_process(t_bsh *bsh)
{
	int		ret;

	ret = 0;
	lexer(bsh->lexer, bsh->input->buffer, INIT);
	connect_tokens(bsh);
	ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
	if (ret == UNCLOSED_QUOTES || ret == END_IS_OP)
	{
		if (bsh->interactive == INTERACTIVE)
		{
			if (handle_unclosed_quotes(bsh->lexer, bsh->input, &ret,
						bsh->tokens) == CATCH_SIGINT)
				return ;
			bsh->tokens[1]->pushed = 0;
			if (ret == ACCEPTED)
				ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
		}
		else
			ft_printf("Missing quotes or end of input is an operator\n");
	}
	add_cmd_to_history(bsh);
	(ret == ACCEPTED && bsh->exec) ? execution(bsh) : 0;
}

static void	file_input(t_bsh *bsh)
{
	int		len;
	char	*line;

	len = 0;
	while (get_next_line(STDIN, &line) > 0)
	{
		len = ft_strlen(line);
		ft_strcpy(bsh->input->buffer, line);
		bsh->input->buffer[len] = '\n';
		start_process(bsh);
		ft_bzero(bsh->input->buffer, len + 1);
		clear_token_list(&bsh->tokens[0]);
		clear_exec(&(bsh->exec));
		ft_strdel(&line);
	}
}

int			main(int argc, char **argv, char **environ)
{
	t_bsh	*bsh;

	(void)argc;
	(void)argv;
	bsh = shell_init(environ);
	if (!(bsh->interactive = isatty(STDIN)))
		file_input(bsh);
	else
	{
		init_termcaps(bsh->term);
		while (42)
		{
			ft_bzero(bsh->input->buffer, bsh->input->buffer_len);
			get_term_size(bsh->term);
			bsh->input->buffer_len = 0;
			print_prompt(bsh->term, BOLD_CYAN);
			wait_for_input(bsh->input, REGULAR_INPUT);
			start_process(bsh);
			clear_token_list(&bsh->tokens[0]);
			clear_exec(&(bsh->exec));
		}
	}
	(bsh->interactive) ? restore_initial_attr(bsh->term) : 0;
	return (0);
}
