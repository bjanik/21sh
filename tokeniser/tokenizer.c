#include "tokenizer.h"
#include "stdio.h"

char	g_op_char[6] = "><&|;";
char	*g_op_list[11] = { ">>", "<<", ">", "<", "&&" , "||", "|", "<&", ">&", ";", NULL};

t_transition	g_aut_token[MAX_STATE][MAX_EVENT] = {
	 {{STD, init},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {BQUOTE, append_char},
	 {BACKSLASH, append_char},
	 {STD, append_char},
	 {STD, NULL},
	 {NEWLINE, NULL},
	 {COMMENT, NULL},
	 {STD, append_char}},

	 {{DQUOTE, NULL},
	 {STD, append_char},
	 {DQUOTE, append_char},
	 {BQUOTE, append_char},
	 {BACKSLASH, append_char},
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
	 {QUOTE, append_char},
	 {QUOTE, append_char}},

	 {{BQUOTE, NULL},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {BACKSLASH, append_char},
	 {STD, append_char},
	 {STD, append_char},
	 {NEWLINE, append_char},
	 {COMMENT, NULL}},
	
	 {{BACKSLASH, NULL},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char},
	 {PREVIOUS, append_char}},
	
	 {{STD, NULL},
	 {DQUOTE, append_char},
	 {QUOTE, append_char},
	 {STD, append_char},
	 {BACKSLASH, append_char},
	 {STD, get_operator},
	 {STD, delimitate_token},
	 {NEWLINE, delimitate_token},
	 {COMMENT, NULL},
	 {STD, append_char}},

	 {{NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL},
	 {NEWLINE, NULL}},

	 {{COMMENT, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL},
	 {NEWLINE, NULL},
	 {COMMENT, NULL},
	 {COMMENT, NULL}},
	};

void	init(t_tokenizer *aut)
{
	//printf("In transition function %s\n", __func__);
}

int		is_operator(char *token)
{
	//printf("In transition function %s\n", __func__);
	int	i;

	i = -1;
	while (g_op_list[++i])
	{
		if (!ft_strcmp(g_op_list[i], token))
			return (1);
	}
	return (0);
}

void	get_operator(t_tokenizer *aut)
{
	if (aut->token_len == 0)
		aut->append_char(aut);
	else if (ft_strchr(g_op_char, aut->current_token[0]))
	{
		aut->current_token[aut->token_len++] = *(aut->input);
		if (!is_operator(aut->current_token))
		{
			aut->current_token[--aut->token_len] = '\0';
			aut->input--;
		}
		aut->delimitate_token(aut);
	}
	else if (!is_operator(aut->current_token))
	{
		aut->delimitate_token(aut);
		aut->append_char(aut);
	}
}

void		get_event(t_tokenizer *aut)
{
	//ft_printf("In transition function %s\n", __func__);
	if (*(aut->input) == '"')
		aut->event.code = EV_DQUOTE;
	else if (*(aut->input) == '\'')
		aut->event.code = EV_QUOTE;
	else if (*(aut->input) == '`')
		aut->event.code = EV_BQUOTE;
	else if (*(aut->input) == '\\')
		aut->event.code = EV_BACKSLASH;
	else if (ft_strchr(g_op_char, *aut->input))
		aut->event.code = EV_OP_CHAR;
	else if (*(aut->input) == '\n')
		aut->event.code = EV_NEWLINE;
	else if (isspace((int)*(aut->input)))
		aut->event.code = EV_BLANK;
	else if (*(aut->input) == '#')
		aut->event.code = EV_COMMENT;
	else if (*(aut->input) != '\0')
		aut->event.code = EV_REG_CHAR;
		
}

void	append_char(t_tokenizer *aut)
{
	if (is_operator(aut->current_token))
	{
		aut->current_token[aut->token_len++] = *(aut->input);
		if (!is_operator(aut->current_token))
		{
			aut->current_token[--aut->token_len] = '\0';
			delimitate_token(aut);
			aut->current_token[aut->token_len++] = *(aut->input);
		}
	}
	else
		aut->current_token[aut->token_len++] = *(aut->input);
}

void	delimitate_token(t_tokenizer *aut)
{
	if (aut->token_len > 0)
		aut->push_back_token(aut);
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
	tk->prev_state = INIT;
	tk->event.code = START;
	tk->token_list = NULL;
	tk->append_char = append_char;
	tk->delimitate_token = delimitate_token;
	tk->push_back_token = push_back_token;
	tk->init_token_node = init_token_node;
	return (tk);
}

void	tokeniser(char *input)
{
	t_tokenizer	*aut;
	e_state		tmp;

	aut = init_tokenizer(input);
	while (aut->state != NEWLINE && *(aut->input) != '\0')
	{
		g_aut_token[aut->state][aut->event.code].p_transition(aut);
		if (aut->state != INIT)
			aut->input++;
		aut->state = g_aut_token[aut->state][aut->event.code].new_state;
		get_event(aut);
	}
	while (aut->token_list)
	{
		ft_printf("%s\n", aut->token_list->token);
		aut->token_list = aut->token_list->next;
	}
		
}

int main()
{
	char input[1000] = "echo >>;>> |c; ||gsdf; -e |><; dsgrez||&&\"\n\"   la'||'| &&>|<>|  compagnie sdfuhgslug[||&&><;;:\n";
	
	tokeniser(input);	
	return (0);
}
