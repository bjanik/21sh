#include "lexer.h"

char	g_op_char[6] = "><&|;";
char	*g_op_list[12] = { ">>", "<<", ">", "<", "&&" , "||", "|", "<&", ">&",
			 ";", "&", NULL};

t_token	*g_token_lst;

t_transition	g_lexer[MAX_STATE][MAX_EVENT] = {
	 {{STD, init},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {BQUOTE, append_char},
	 {STD, append_char},
	 {STD, NULL},
	 {NWLINE, NULL},
	 {COMMENT, NULL},
	 {STD, append_char}},

	 {{DQUOTE, append_char},
	 {STD, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, handle_backslash}},

	 {{QUOTE, NULL},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char}},

	 {{STD, NULL},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {BQUOTE, NULL},
	 {STD, append_char},
	 {STD, append_char},
	 {STD, append_char},
	 {NWLINE, append_char},
	 {COMMENT, NULL},
	 {STD, handle_backslash}},
	
	 {{STD, append_char},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {STD, get_operator},
	 {STD, delimitate_token},
	 {NWLINE, end_of_input},
	 {COMMENT, skip_char},
	 {STD, append_char},
	 {STD, handle_backslash}},

	 {{NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL},
	 {NWLINE, NULL}},

	 {{COMMENT, NULL},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char},
	 {NWLINE, end_of_input},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char},
	 {COMMENT, skip_char}},
	};

void	init(t_lexer *lexer)
{
}

void	skip_char(t_lexer *lexer)
{
}

int		is_operator(char *token)
{
	int	i;

	i = -1;
	while (g_op_list[++i])
	{
		if (!ft_strcmp(g_op_list[i], token))
			return (i);
	}
	return (-1);
}

void	get_operator(t_lexer *lexer)
{
	if (lexer->token_len == 0)
		append_char(lexer);
	else if (ft_strchr(g_op_char, lexer->current_token[0]))
	{
		lexer->current_token[lexer->token_len++] = *(lexer->input);
		if (is_operator(lexer->current_token) == -1)
		{
			lexer->current_token[--lexer->token_len] = '\0';
			lexer->input--;
		}
		delimitate_token(lexer);
	}
	else if (is_operator(lexer->current_token) == -1)
	{
		delimitate_token(lexer);
		append_char(lexer);
	}
}

void		handle_backslash(t_lexer *lexer)
{
	lexer->current_token[lexer->token_len++] = *(lexer->input);
	lexer->input++;
	lexer->current_token[lexer->token_len++] = *(lexer->input);
}
void		get_event(t_lexer *lexer)
{
	char	c;

	c = *(lexer->input);
	if (c == '"')
		lexer->event = EV_DQUOTE;
	else if (c == '\\')
		lexer->event = EV_BACKSLASH;
	else if (c == '\'')
		lexer->event = EV_QUOTE;
	else if (c == '`')
		lexer->event = EV_BQUOTE;
	else if (ft_strchr(g_op_char, c))
		lexer->event = EV_OP_CHAR;
	else if (c == '\n')
		lexer->event = EV_NEWLINE;
	else if (ft_isspace(c))
		lexer->event = EV_BLANK;
	else if (c == '#' && !lexer->token_len)
		lexer->event = EV_COMMENT;
	else if (c != '\0')
		lexer->event = EV_REG_CHAR;
}

void	realloc_current_token(t_lexer *lexer)
{
	char    *tmp;

        tmp = lexer->current_token;
	lexer->token_size *= 2;
        lexer->current_token = (char *)ft_malloc((lexer->token_size * 2 + 1)
			* sizeof(char));
	ft_bzero(lexer->current_token, lexer->token_size + 1);
	ft_strcpy(lexer->current_token, tmp);
	free(tmp);
}

void	append_char(t_lexer *lexer)
{
	if (lexer->token_len + 1 > lexer->token_size)
		realloc_current_token(lexer);
	if (is_operator(lexer->current_token) != -1)
	{
		lexer->current_token[lexer->token_len++] = *(lexer->input);
		if (is_operator(lexer->current_token) == -1)
		{
			lexer->current_token[--lexer->token_len] = '\0';
			delimitate_token(lexer);
			lexer->current_token[lexer->token_len++] = *(lexer->input);
		}
	}
	else
		lexer->current_token[lexer->token_len++] = *(lexer->input);
}

void	delimitate_token(t_lexer *lexer)
{
	t_token	*token;
	
	if (lexer->token_len > 0)
	{
		token = init_token_node(lexer);
		if (!lexer->token_list)
		{
			token->prev = NULL;
			lexer->token_list = token;
		}
		else
		{
			lexer->last_token->next = token;
			token->prev = lexer->last_token;
		}
		lexer->last_token = token;
	}
	//ft_printf("%p && %s\n", lexer->token_list, lexer->last_token->token);
}	

void	end_of_input(t_lexer *lexer)
{
	delimitate_token(lexer);
	append_char(lexer);
	delimitate_token(lexer);
}

t_lexer	*init_lexer(t_lexer *lexer, char *input, int initial_state)
{
	if (!lexer)
	{
		if (!(lexer = (t_lexer*)malloc(sizeof(t_lexer))))
			return (NULL);
		if (!(lexer->current_token = (char*)malloc(INITIAL_TOKEN_SIZE + 1)))
			return (NULL);
	}
	lexer->input = ft_strdup(input);
	ft_bzero(lexer->current_token, INITIAL_TOKEN_SIZE);
	lexer->token_size = INITIAL_TOKEN_SIZE;
	lexer->token_len = 0;
	lexer->state = initial_state;
	lexer->event = START;
	lexer->token_list = NULL;
	lexer->last_token = NULL;
	return (lexer);
}

void    get_io_number(t_token *token_lst)
{
        t_token *lst;

        lst = token_lst;
        while (lst)
        {
                if (lst->type == WORD
				&& ft_str_isdigit(lst->token)
				&& lst->next
				&& (lst->next->type >= DLESS
                                && lst->next->type <= GREAT))
                        lst->type = IO_NUMBER;
                lst = lst->next;
        }
}

t_lexer	*lexer(t_lexer *lexer, char *input, int initial_state)
{
	char	*buffer;
	
	lexer = init_lexer(lexer, input, initial_state);
	buffer = lexer->input;
	if (initial_state != INIT)
		get_event(lexer);
	while (lexer->state != NWLINE && *(lexer->input) != '\0')
	{
		g_lexer[lexer->state][lexer->event].p_transition(lexer);
		if (lexer->state != INIT)
			lexer->input++;
		lexer->state = g_lexer[lexer->state][lexer->event].new_state;
		get_event(lexer);
	}
	lexer->input = buffer;	
	delimitate_token(lexer);
	get_io_number(lexer->token_list);
	//display_token_list(input, lexer->token_list);
	//ft_printf("END LEXER \n");
	//ft_printf("%d\n", lexer->state);
	return (lexer);
}
