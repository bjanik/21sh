/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:15:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/16 17:46:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int			init_termcaps(void)
{
	char			*termtype;
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

void			clear_token_list(t_token *token_lst)
{
	t_token		*lst;

	lst = token_lst;
	while (lst)
	{
		token_lst = token_lst->next;
		ft_strdel(&lst->token);
		lst->next = NULL;
		ft_memdel((void**)lst);
		lst = token_lst;
	}
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
	}
}

void	del(void *content, size_t size)
{
	(void)size;
	ft_memdel(&content);
}

void		clear_exec(t_exec **exec)
{
	t_exec	*ex;
	t_redir	*rd;
	t_list	*wd;

	if (!exec || !*exec)
		return ;
	ex = *exec;
	wd = ex->word_list;
	while (ex)
	{
		/*while (ex->word_list)
		{
			ex->word_list = ex->word_list->next;
			ft_memdel(&wd->content);
			ft_memdel((void**)&wd);
			wd = ex->word_list;
		}*/
		ft_lstdel(&ex->word_list, del);
		ex->last_word = NULL;
		rd = ex->redir_list;
		ex->prev = NULL;
		ex->last_redir = NULL;
		while (rd)
		{
			ex->redir_list = rd->next;
			ft_strdel(&rd->dest_file);
			ft_strdel(&rd->here_end);
			rd->next = NULL;
			rd = ex->redir_list;
		}
		ex = ex->next;
		ft_memdel((void**)exec);
		*exec = ex;
	}
}

static int		run_builtin(t_bsh *bsh, int builtin, char **cmd)
{
	handle_redirection(bsh->exec);
	bsh->exec->exit_status = g_builtins[builtin].builtin(&(bsh->env), cmd);
}

void			execution(t_bsh *bsh, int ret)
{
	char	**cmd;
	int		builtin;

	cmd = NULL;
	builtin = 0;
	while (bsh->exec && ret == ACCEPTED)
	{
		if (bsh->exec->word_list)
			cmd = lst_to_tab(bsh->exec->word_list, bsh->exec->word_count);
		if ((builtin = cmd_is_builtin(cmd)) > 0)
			run_builtin(bsh, builtin, cmd);
		else
			launch_command(bsh->exec, bsh->env, cmd);
		if ((bsh->exec->cmd_separator == AND_IF && bsh->exec->exit_status) ||
			(bsh->exec->cmd_separator == OR_IF && !bsh->exec->exit_status))
			bsh->exec = bsh->exec->next;
		if (bsh->exec)
			bsh->exec = bsh->exec->next;
	}
}

int		main(int argc, char **argv, char **environ)
{
	t_bsh		*bsh;
	int			ret;

	bsh = get_bsh();
	bsh->env = env_to_lst(environ);
	init_termcaps();
	while (42)
	{
		ft_bzero(bsh->input->buffer, bsh->input->buffer_size);
		bsh->input->buffer_len = 0;
		bsh->term->print_prompt(bsh->term, BOLD_CYAN);
		waiting_for_input(bsh->input);
		bsh->lexer = lexer(bsh->lexer, bsh->input->buffer, INIT);
		bsh->tokens[0] = bsh->lexer->token_list;
		bsh->tokens[1] = bsh->lexer->last_token;
		ret = parser(&(bsh->exec), bsh->lexer->token_list, SAVE_EXEC);
		if (ret == UNCLOSED_QUOTES || ret == END_IS_OP)
		{
			handle_unclosed_quotes(bsh->lexer, bsh->input, ret, bsh->tokens);
			bsh->tokens[1]->pushed = 0;
			ret = parser(&(bsh->exec), bsh->tokens[0], SAVE_EXEC);
		}
		/*ft_printf("%p\n", bsh->lexer);
		ft_printf("%p\n", bsh->exec->word_list->content);
		ft_printf("%p\n", bsh->tokens[0]->token);
		ft_printf("%p\n", bsh->input->buffer);
		ft_printf("%p\n", bsh->lexer->input);
		ft_printf("%p\n", bsh->lexer->current_token);*/
		ft_strdel(&bsh->lexer->input);
		clear_token_list(bsh->tokens[0]);
		clear_token_list(bsh->lexer->token_list);
		execution(bsh, ret);
		clear_exec(&(bsh->exec));
	}
	return (0);
}
