/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/30 16:06:04 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void			waiting_for_input(t_input *input)
{
	while (42)
	{
		ft_bzero(input->read_buffer, MAX_KEY_LENGTH);
		if (read(STDIN, input->read_buffer, MAX_KEY_LENGTH) < 1)
			exit(EXIT_FAILURE);
		if (get_key(input))
			break ;
	}
	write(STDOUT, RETURN, 1);
}

/*void			display_token_list(t_input *input, t_token *lst)
{
	while (lst)
	{
		ft_printf("[%s] type =  %d\n", lst->token, lst->type);
		lst = lst->next;
	}
	ft_printf("----------------------------------\n");
}*/

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
		lexer(lex, input->buffer, lex->state);
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
		input->buffer = ft_strjoin_free(input->buf_tmp, input->buffer, 0);
	}
}

void			execution(t_bsh *bsh)
{
	char	**cmd;
	int		builtin;
	t_exec	*exec;
	int		**pipes_fd;
	int		nb_pipes;
	int		pid;

	cmd = NULL;
	pipes_fd = NULL;
	builtin = -1;
	exec = bsh->exec;
	while (exec)
	{
		if ((pipes_fd = get_pipes_fd(exec, &nb_pipes)))
			pipe_sequence(&exec, pipes_fd, nb_pipes);
		else
		{
			if (exec->word_list)
			{
				cmd = lst_to_tab(exec->word_list, exec->word_count);
				expand_words(bsh->exp, cmd);
			}
			if ((builtin = cmd_is_builtin(cmd)) > -1)
				run_builtin(builtin, cmd);
			else
			{
				if ((pid = fork()) < 0)
					exit(EXIT_FAILURE);
				if (!pid)
					run_binary(exec, bsh->env, cmd);
			}
			ft_free_string_tab(&cmd);
			waitpid(pid, &exec->exit_status, 0);
			restore_custom_attr(get_bsh()->term);
			if (WIFEXITED(exec->exit_status))
				get_bsh()->exit_status = WEXITSTATUS(exec->exit_status);
			if ((exec->cmd_separator == AND_IF && bsh->exit_status) ||
				(exec->cmd_separator == OR_IF && !bsh->exit_status))
				exec = exec->next;
			if (exec)
				exec = exec->next;
		}
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
	(ret == ACCEPTED) ? execution(bsh): 0;
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
	bsh->exp = init_expander(bsh->env);
	if (argc > 1)
		file_mode(bsh, argv);
	else
	{
		init_termcaps(bsh->term);
		while (42)
		{
			ft_bzero(bsh->input->buffer, bsh->input->buffer_size);
			bsh->input->buffer_len = 0;
			print_prompt(bsh->term, BOLD_CYAN);
			waiting_for_input(bsh->input);
			start_process(bsh, INTERACTIVE);
			clear_token_list(&bsh->tokens[0]);
			bsh->lexer->token_list = NULL;
			bsh->lexer->last_token = NULL;
			//clear_token_list(&bsh->lexer->token_list);
			clear_exec(&(bsh->exec));
		}
	}
	return (0);
}
