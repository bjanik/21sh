/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/07 15:55:25 by bjanik           ###   ########.fr       */
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

static void	start_process(t_bsh *bsh, int mode)
{
	int		ret;

	ret = 0;
	lexer(bsh->lexer, bsh->input->buffer, INIT);
	connect_tokens(bsh);
	ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
	if (ret == UNCLOSED_QUOTES || ret == END_IS_OP)
	{
		if (mode == INTERACTIVE)
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

static void	file_mode(t_bsh *bsh, char **argv)
{
	int		len;
	char	*line;
	int		fd;

	len = 0;
	if (access(argv[1], F_OK))
		exit(ft_no_file_msg(argv[1]));
	else if (access(argv[1], R_OK))
		exit(ft_perm_denied_msg(argv[1]));
	fd = open(argv[1], O_RDONLY, 0644);
	while (get_next_line(fd, &line) > 0)
	{
		len = ft_strlen(line);
		ft_strcpy(bsh->input->buffer, line);
		bsh->input->buffer[len] = '\n';
		start_process(bsh, FILE_MODE);
		ft_bzero(bsh->input->buffer, len + 1);
		clear_token_list(&bsh->tokens[0]);
		clear_exec(&(bsh->exec));
		ft_strdel(&line);
	}
}

int			main(int argc, char **argv, char **environ)
{
	t_bsh	*bsh;

	bsh = shell_init(environ);
	init_termcaps(bsh->term);
	if (argc > 1)
		file_mode(bsh, argv);
	else
	{
		while (42)
		{
			ft_bzero(bsh->input->buffer, bsh->input->buffer_len);
			get_term_size(bsh->term);
			bsh->input->buffer_len = 0;
			print_prompt(bsh->term, BOLD_CYAN);
			wait_for_input(bsh->input, REGULAR_INPUT);
			start_process(bsh, INTERACTIVE);
			clear_token_list(&bsh->tokens[0]);
			clear_exec(&(bsh->exec));
		}
	}
	restore_initial_attr(bsh->term);
	return (0);
}
