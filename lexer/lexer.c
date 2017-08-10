#include "lexer.h"

char	g_op_char[6] = "><&|;";
char	*g_op_list[11] = { ">>", "<<", ">", "<", "&&" , "||", "|", "<&", ">&", ";", NULL};

t_token	*g_token_lst;


t_transition	g_aut_lexer[MAX_STATE][MAX_EVENT] = {
	 {{STD, init},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {BQUOTE, append_char},
	 {STD, append_char},
	 {STD, NULL},
	 {NWLINE, NULL},
	 {COMMENT, NULL},
	 {STD, append_char}},

	 {{DQUOTE, NULL},
	 {STD, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char},
	 {DQUOTE, append_char}},

	 {{QUOTE, NULL},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char},
	 {QUOTE, append_char}},

	 {{BQUOTE, NULL},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {STD, append_char},
	 {STD, append_char},
	 {NWLINE, append_char},
	 {COMMENT, NULL}},
	
	 {{STD, NULL},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {STD, get_operator},
	 {STD, delimitate_token},
	 {NWLINE, end_of_input},
	 {COMMENT, skip_char},
	 {STD, append_char}},

	 {{NWLINE, NULL},
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
	 {COMMENT, skip_char}},
	};

void	init(t_tokenizer *lexer)
{
}

void	skip_char(t_tokenizer *lexer)
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

void	get_operator(t_tokenizer *lexer)
{
	if (lexer->token_len == 0)
		lexer->append_char(lexer);
	else if (ft_strchr(g_op_char, lexer->current_token[0]))
	{
		lexer->current_token[lexer->token_len++] = *(lexer->input);
		if (is_operator(lexer->current_token) == -1)
		{
			lexer->current_token[--lexer->token_len] = '\0';
			lexer->input--;
		}
		lexer->delimitate_token(lexer);
	}
	else if (is_operator(lexer->current_token) == -1)
	{
		lexer->delimitate_token(lexer);
		lexer->append_char(lexer);
	}
}

void		get_event(t_tokenizer *lexer)
{
	char	c;

	c = *(lexer->input);
	if (c == '"')
		lexer->event = EV_DQUOTE;
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

void	append_char(t_tokenizer *lexer)
{
	if (is_operator(lexer->current_token) != -1)
	{
		lexer->current_token[lexer->token_len++] = *(lexer->input);
		if (is_operator(lexer->current_token) == -1)
		{
			lexer->current_token[--lexer->token_len] = '\0';
			lexer->delimitate_token(lexer);
			lexer->current_token[lexer->token_len++] = *(lexer->input);
		}
	}
	else
		lexer->current_token[lexer->token_len++] = *(lexer->input);
}

void	delimitate_token(t_tokenizer *lexer)
{
	if (lexer->token_len > 0)
		push_back_token(lexer);
}	

void	end_of_input(t_tokenizer *lexer)
{
	lexer->delimitate_token(lexer);
	lexer->append_char(lexer);
	lexer->delimitate_token(lexer);
}

t_tokenizer	*init_tokenizer(char *input)
{
	t_tokenizer	*tk;
	
	if (!(tk = (t_tokenizer*)malloc(sizeof(t_tokenizer))))
		return (NULL);
	tk->input = ft_strdup(input);
	if (!(tk->current_token = (char*)malloc(512 + 1)))
		return (NULL);
	ft_bzero(tk->current_token, 512);
	tk->token_len = 0;
	tk->state = INIT;
	tk->event = START;
	tk->token_list = NULL;
	tk->append_char = append_char;
	tk->skip_char = skip_char;
	tk->delimitate_token = delimitate_token;
	return (tk);
}

t_token	*tokenizer(char *input)
{
	t_tokenizer	*lexer;
	
	lexer = init_tokenizer(input);
	while (lexer->state != NWLINE && *(lexer->input) != '\0')
	{
		g_aut_lexer[lexer->state][lexer->event].p_transition(lexer);
		if (lexer->state != INIT)
			lexer->input++;
		lexer->state = g_aut_lexer[lexer->state][lexer->event].new_state;
		get_event(lexer);
	}
	return (lexer->token_list);
}

/*int	yylex(void)
{
	t_token	*tmp;

	tmp = g_token_lst;
	if (g_token_lst != NULL)	
	{
		g_token_lst = g_token_lst->next;
		if (tmp->type == WORD)
			yylval.s = ft_strdup(tmp->token);
		return (tmp->type);
	}
	return (0);
}*/
