/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/08 19:31:22 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void			display_token_list(t_input *input, t_token *lst)
{
	t_token	*l;
	l = lst;
	while (l)
	{
		dprintf(input->fd, "[%s] type =  %d\n", l->token, l->type);
		l = l->next;
	}
}

void		del_newline_token(t_lexer *lexer, t_token **token_lst)
{
	t_token	*tk;
	t_bsh	*bsh;

	bsh = get_bsh();
	tk = *token_lst;
	lexer->state = STD;
	*token_lst = tk->prev;
	ft_strdel(&tk->token);
	ft_memdel((void**)tk);
	(*token_lst)->next = NULL;
	bsh->input->buffer[--bsh->input->buffer_len] = '\0';
}

void			handle_unclosed_quotes(t_lexer *lex, t_input *input, int *ret,
		t_token *tokens[])
{
	while (*ret == END_IS_OP || *ret == UNCLOSED_QUOTES)
	{
		(*ret == END_IS_OP)? del_newline_token(lex, &tokens[1]): 0;
		input->buf_tmp = input->buffer;
		input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
		display_prompt(input);
		waiting_for_input(input);
		lexer(lex, input->buffer, lex->state);
		*ret = parser(NULL, lex->token_list[0], NO_SAVE_EXEC);
		if ((*ret == UNCLOSED_QUOTES || *ret == ACCEPTED) &&
			tokens[1]->type == WORD)
		{
			input->buffer[--input->buffer_len] = '\0';
			tokens[1]->token = ft_strjoin_free(tokens[1]->token,
					lex->token_list[0]->token, 3);
			tokens[1]->pushed = 0;
			if (lex->token_list[0]->next)
			{
				tokens[1]->next = lex->token_list[0]->next;
				lex->token_list[0] = lex->token_list[0]->next;
				tokens[1] = lex->token_list[1];
				ft_memdel((void**)&lex->token_list[0]->prev);
			}
		}
		else if (*ret != SYNTAX_ERROR)
		{
			tokens[1]->next = lex->token_list[0];
			tokens[1] = lex->token_list[1];
		}
		input->buffer = ft_strjoin_free(input->buf_tmp, input->buffer, 1);
		input->buffer_len = ft_strlen(input->buffer);
	}
}

static void	start_process(t_bsh *bsh, int mode)
{
	int		ret;

	ret = 0;
	lexer(bsh->lexer, bsh->input->buffer, INIT);
	bsh->tokens[0] = bsh->lexer->token_list[0];
	bsh->tokens[1] = bsh->lexer->token_list[1];
	bsh->lexer->token_list[0] = NULL;
	bsh->lexer->token_list[1] = NULL;
	display_token_list(bsh->input, bsh->tokens[0]);
	ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
	if (ret == UNCLOSED_QUOTES || ret == END_IS_OP)
	{
		if (mode == INTERACTIVE)
		{
			handle_unclosed_quotes(bsh->lexer, bsh->input, &ret, bsh->tokens);
			bsh->tokens[1]->pushed = 0;
			if (ret != SYNTAX_ERROR)
				ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
		}
		else
			ft_printf("Missing closing quotes or end of input is an operator\n");
	}
	bsh->input->buffer[--bsh->input->buffer_len] = '\0';
	if (bsh->input->buffer_len > 0)
		append_history(bsh->history, bsh->input->buffer,
				bsh->input->buffer_len + 1);
	(ret == ACCEPTED && bsh->exec->word_list) ? execution(bsh): 0;
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
			ft_bzero(bsh->input->buffer, bsh->input->buffer_size);
			bsh->input->buffer_len = 0;
			print_prompt(bsh->term, BOLD_CYAN);
			waiting_for_input(bsh->input);
			start_process(bsh, INTERACTIVE);
			clear_token_list(&bsh->tokens[0]);
			clear_exec(&(bsh->exec));
		}
	}
	restore_initial_attr(bsh->term);
	return (0);
}
