#include "tokenizer.h"


t_token	*init_token_node(t_tokenizer *aut)
{
	t_token	*tk;

	tk = (t_token*)malloc(sizeof(t_token));
	tk->token = ft_strdup(aut->current_token);
	ft_bzero(aut->current_token, aut->token_len);
	aut->token_len = 0;
	tk->next = NULL;
	return (tk);	
}

void	push_back_token(t_tokenizer *aut)
{
	t_token	*tk;
	t_token	*tmp;
	
	tk = init_token_node(aut);
	tmp = aut->token_list;
	if (!aut->token_list)
		aut->token_list = tk;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = tk;
	}
}
