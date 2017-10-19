/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 19:32:30 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int				init_termcaps(void)
{
	char		*termtype;
	t_termios	term;

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

void			waiting_for_input(t_input *input)
{
	while (42)
	{
		ft_bzero(input->read_buffer, MAX_KEY_LENGTH);
		if (read(STDIN, input->read_buffer, MAX_KEY_LENGTH) < 1)
			exit(-1);
		if (get_key(input))
			break ;
	}
	write(STDOUT, RETURN, 1);
}

void			display_token_list(t_input *input, t_token *lst)
{
	while (lst)
	{
		ft_printf("[%s] type =  %d\n", lst->token, lst->type);
		lst = lst->next;
	}
	ft_printf("----------------------------------\n");
}

void	del_newline_token(t_lexer *lexer, t_token **token_lst)
{
	t_token	*tk;

	tk = *token_lst;
	lexer->state = STD;
	*token_lst = tk->prev;
	ft_strdel(&tk->token);
	ft_memdel((void**)tk);
	(*token_lst)->next = NULL;
}

void			handle_unclosed_quotes(t_lexer *lex, t_input *input, int ret,
		t_token *token_lst[])
{
	while (ret == END_IS_OP || ret == UNCLOSED_QUOTES)
	{
		(ret == END_IS_OP)? del_newline_token(lex, &token_lst[1]): 0;
		input->buf_tmp = input->buffer;
		input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
		display_prompt(input);
		waiting_for_input(input);
		lex = lexer(lex, input->buffer, lex->state);
		ret = parser(NULL, lex->token_list, NO_SAVE_EXEC);
		if ((ret == UNCLOSED_QUOTES || ret == ACCEPTED) &&
			token_lst[1]->type == WORD)
		{
			token_lst[1]->token = ft_strjoin_free(token_lst[1]->token,
					lex->token_list->token, 3);
			token_lst[1]->pushed = 0;
			if (lex->token_list->next)
			{
				token_lst[1]->next = lex->token_list->next;
				lex->token_list = lex->token_list->next;
				token_lst[1] = lex->last_token;
				ft_memdel((void**)&lex->token_list->prev);
			}
		}
		else if (ret != SYNTAX_ERROR)
		{
			token_lst[1]->next = lex->token_list;
			token_lst[1] = lex->last_token;
		}
		input->buffer = ft_strjoin_free(input->buf_tmp, input->buffer, 1);
	}
}

static void		run_builtin(t_bsh *bsh, int builtin, char **cmd)
{
	handle_redirection(bsh->exec);
	bsh->exec->exit_status = g_builtins[builtin].builtin(&(bsh->env), cmd);
}

void			execution(t_bsh *bsh, int ret)
{
	char	**cmd;
	int		builtin;
	t_exec	*exec;

	cmd = NULL;
	builtin = 0;
	exec = bsh->exec;
	while (exec && ret == ACCEPTED)
	{
		if (exec->word_list)
			cmd = lst_to_tab(exec->word_list, exec->word_count);
		if ((builtin = cmd_is_builtin(cmd)) > 0)
			run_builtin(bsh, builtin, cmd);
		else
			launch_command(exec, bsh->env, cmd);
		if ((exec->cmd_separator == AND_IF && exec->exit_status) ||
			(exec->cmd_separator == OR_IF && !exec->exit_status))
			exec = exec->next;
		if (exec)
			exec = exec->next;
	}
}

static void	start_process(t_bsh *bsh, int mode)
{
	int	ret;

	ret = 0;
	bsh->lexer = lexer(bsh->lexer, bsh->input->buffer, INIT);
	bsh->tokens[0] = bsh->lexer->token_list;
	bsh->tokens[1] = bsh->lexer->last_token;
	ret = parser(&(bsh->exec), bsh->lexer->token_list, SAVE_EXEC);
	if (ret == UNCLOSED_QUOTES || ret == END_IS_OP)
	{
		if (mode == INTERACTIVE)
		{
			handle_unclosed_quotes(bsh->lexer, bsh->input, ret, bsh->tokens);
			bsh->tokens[1]->pushed = 0;
			ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
		}
		else
			ft_printf("Missing closing quotes or end of input is an operator\n");
	}
	bsh->input->buffer_len = ft_strlen(bsh->input->buffer);
	bsh->input->buffer[--bsh->input->buffer_len] = '\0';
	if (bsh->input->buffer_len > 0)
		append_history(bsh->history, bsh->input->buffer,
				bsh->input->buffer_len + 1);
	execution(bsh, ret);
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
		clear_token_list(&bsh->lexer->token_list);
		clear_exec(&(bsh->exec));
		ft_strdel(&line);
	}
}

int		main(int argc, char **argv, char **environ)
{
	t_bsh		*bsh;

	bsh = get_bsh();
	bsh->env = env_to_lst(environ);
	if (argc > 1)
		file_mode(bsh, argv);
	else
	{
		init_termcaps();
		while (42)
		{
			ft_bzero(bsh->input->buffer, bsh->input->buffer_size);
			bsh->input->buffer_len = 0;
			bsh->term->print_prompt(bsh->term, BOLD_CYAN);
			waiting_for_input(bsh->input);
			start_process(bsh, INTERACTIVE);
			clear_token_list(&bsh->tokens[0]);
			clear_token_list(&bsh->lexer->token_list);
			clear_exec(&(bsh->exec));
		}
	}
	return (0);
}
