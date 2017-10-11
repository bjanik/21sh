#include "libft.h"
#include "lexer.h"
#include "parser.h"
//#include "expander.h"
#include "history.h"
#include "input.h"
#include "builtins.h"
#include "exec.h"

int             init_termcaps(void)
{
        char                    *termtype;
        struct termios  term;

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

void		clear_token_list(t_token *token_lst)
{
	t_token	*lst;

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

void		waiting_for_input(t_input *input)
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

void		display_token_list(t_input *input, t_token *lst)
{
	while (lst)
	{
		dprintf(input->fd, "[%s] type =  %d\n", lst->token, lst->type);
		lst = lst->next;
	}
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

void	handle_unclosed_quotes(t_lexer *lex, t_input *input, int ret, t_token *token_lst[])
{
	while (ret == -2 || ret == -3)
	{
		(ret == -2)? del_newline_token(lex, &token_lst[1]): 0;
		input->buf_tmp = input->buffer;
		input->buffer = (char*)ft_memalloc(INITIAL_BUFFER_SIZE + 1);
		display_prompt(input);
		waiting_for_input(input);
		lex = lexer(lex, input->buffer, lex->state);
		ret = parser(NULL, lex->token_list, NO_SAVE_EXEC);
		if ((ret == -3 || ret == -1) && token_lst[1]->type == WORD)
		{
			token_lst[1]->token = ft_strjoin_free(token_lst[1]->token, lex->token_list->token, 3);
			token_lst[1]->pushed = 0;
			if (lex->token_list->next)
			{
				token_lst[1]->next = lex->token_list->next;
				lex->token_list = lex->token_list->next;
				token_lst[1] = lex->last_token;
				ft_memdel((void**)&lex->token_list->prev);
			}
		}
		else
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
		while (ex->word_list)
		{
			ex->word_list = ex->word_list->next;
			ft_memdel(&wd->content);
			ft_memdel((void**)&wd);
			wd = ex->word_list;
		}
		//ft_lstdel(&ex->word_list, del);
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

int		main(int argc, char **argv, char **environ)
{
	t_token		*token_lst[2];
	t_history	*history;
	t_input		*input;
	t_term		*term;
	t_lexer		*lex;
	t_exec		*exec;
	t_env		*env;
	int		ret;

	term = init_term();
        history = init_history();
        input = init_input(term, history);
	lex = NULL;
	exec = NULL;
        init_termcaps();
	env = dup_env(environ);
	while (42)
	{
		ft_bzero(input->buffer, input->buffer_size);
		input->buffer_len = 0;
		term->print_prompt(term, BOLD_CYAN);
		waiting_for_input(input);
		lex = lexer(lex, input->buffer, INIT);
		token_lst[0] = lex->token_list;
		token_lst[1] = lex->last_token;
		display_token_list(input, token_lst[0]);
		ret = parser(&exec, lex->token_list, SAVE_EXEC);
		if (ret < -1 && ret != -4)
		{
			handle_unclosed_quotes(lex, input, ret, token_lst);
			token_lst[1]->pushed = 0;
			display_token_list(input, token_lst[0]);
			ret = parser(&exec, token_lst[0], SAVE_EXEC);
		}
		//display_token_list(input, token_lst[0]);
		clear_token_list(token_lst[0]);
		while (exec && ret == -1)
		{
			launch_command(exec, env);
			if ((exec->cmd_separator == AND_IF && exec->exit_status) ||
				(exec->cmd_separator == OR_IF && !exec->exit_status))
				exec = exec->next;
			if (exec)
				exec = exec->next;
		}
		clear_exec(&exec);
	}
}
