#include "expander.h"
#include "history.h"
#include "libft.h"

void	init_exp_aut(t_expander *exp)
{
}

t_expander	*init_expander(char **env, t_history *history, t_token *tk_lst)
{
	t_expander *exp;

	if ((exp = (t_expander*)malloc(sizeof(t_expander))) == NULL)
		return (NULL);
	exp->state = INIT1;
	exp->event = START1;
	exp->history = history;
	exp->env = env;
	exp->token_lst = tk_lst;
	exp->buffer = (char *)ft_memalloc(INITIAL_SIZE + 1);
	exp->buffer_len = 0;
	exp->append = append;
	exp->handle_tilde = handle_tilde;
	exp->handle_backslash = handle_backslash;
	exp->handle_dollar = handle_dollar;
	exp->tmp = exp->token_lst->token;
	return (exp); 
}

void	get_event_1(t_expander *exp)
{
	if (*(exp->tmp) == '"')
		exp->event = EV_DQUOTE1;
	else if (*(exp->tmp) == '\'')
		exp->event = EV_QUOTE1;
	else if (*(exp->tmp) == '$')
		exp->event = EV_DOLLAR;
	else if (*(exp->tmp) == '!')
		exp->event = EV_BANG;
	else if (*(exp->tmp) == '~')
		exp->event = EV_TILDE;
	else if (*(exp->tmp) == '\\')
		exp->event = EV_BACKSLASH;
	else if (*(exp->tmp) != '\0')
		exp->event = EV_REG_CHAR1;
}

void	save_expanded_token(t_expander *exp, char *s)
{
	exp->token_lst->token = ft_strdup(exp->buffer);
	ft_bzero(exp->buffer, INITIAL_SIZE);
	exp->buffer_len = 0;
	ft_strdel(&s);
	(exp->token_lst->next) ? exp->tmp = exp->token_lst->next->token : 0;
	get_event_1(exp);
	exp->token_lst = exp->token_lst->next;
}

void	expander(char **env,t_history *history, t_token *tk_lst)
{
	t_expander	*exp;
	char		*s;

	exp = init_expander(env, history, tk_lst);
	while (exp->token_lst)
	{
		s = exp->token_lst->token;
		while (*(exp->tmp))
		{
			g_aut_expander[exp->state][exp->event].p_transit(exp);
			(exp->state != INIT1) ? *(exp->tmp)++ : 0;
			exp->state = g_aut_expander[exp->state][exp->event].new_state;
			if(*(exp->tmp) == '\0')
				break ;
			get_event_1(exp);
		}
		save_expanded_token(exp, s);
	}
}

void	append(t_expander *exp)
{
	exp->buffer[exp->buffer_len++] = *(exp->tmp);
}

void	skip(t_expander *exp)
{
}

void	handle_tilde(t_expander *exp)
{
	char	*home;

	home = NULL;
	if (exp->buffer_len == 0 && (*(exp->tmp + 1) == '/' || !*(exp->tmp + 1)))
	{
		if ((home = getenv("HOME")))
		{
			ft_strcpy(exp->buffer, home);
			exp->buffer_len += ft_strlen(home);
		}
	}
	else
		exp->append(exp);
}

void	handle_backslash(t_expander *exp)
{
	exp->tmp++;
	exp->append(exp);
}

static char	*get_special_parameter(char *s, int len)
{
	if (len == 0)
	{
		if (*(s - 1) == '$')
			s = ft_strdup(ft_itoa((long long)getpid()));
	}
	return (s);
}

void	handle_dollar(t_expander *exp)
{
	char	*s;
	char	*s2;
	int	len;
	
	s = exp->tmp + 1;
	if (!*s)
	{
		exp->append(exp);
		return ;
	}
	len = 0;
	while (ft_isdigit(*s) || ft_isalpha(*s))
	{
		len++;
		s++;
	}
	s = ft_strndup(exp->tmp + 1, len);
	if ((s2 = getenv(s)))
	{
		ft_strcat(exp->buffer, s2);
		exp->buffer_len += ft_strlen(s2);
	} 
	exp->tmp += len;
	free(s);
}

void	handle_bang(t_expander *exp)
{
	char	*s;
	int	nb;
	
	s = exp->tmp + 1;
	if (*s == '\0')
		exp->append(exp);
	if (*s == '!')
	{
		ft_strcat(exp->buffer, exp->history->history[exp->history->len]);
		exp->buffer_len += ft_strlen(exp->history->history[exp->history->len]);
		exp->tmp++;
	}
	else if (!ft_isdigit(*s))
	{
		
	}
	nb = ft_atoi(s);
		
	
	
}

t_transit g_aut_expander[MAX_STATE1][MAX_EVENT1] = {
	{{STD1, init_exp_aut},
	{DQUOTE1, skip},
	{QUOTE1, skip},
	{STD1, handle_dollar},
	{STD1, handle_bang},
	{STD1, handle_tilde},
	{STD1, handle_backslash},
	{STD1, append}},
	
	{{STD1, NULL},
	{DQUOTE1, skip},
	{QUOTE1, skip},
	{STD1, handle_dollar},
	{STD1, handle_bang},
	{STD1, handle_tilde},
	{STD1, handle_backslash},
	{STD1, append}},

	{{STD1, NULL},
	{STD1, skip},
	{DQUOTE1, append},
	{DQUOTE1, handle_dollar},
	{DQUOTE1, handle_bang},
	{DQUOTE1, append},
	{DQUOTE1, handle_backslash},
	{DQUOTE1, append}},

	{{QUOTE1, NULL},
	{QUOTE1, append},
	{STD1, skip},
	{QUOTE1, append},
	{QUOTE1, append},
	{QUOTE1, append},
	{QUOTE1, append},
	{QUOTE1, append}},
	};
