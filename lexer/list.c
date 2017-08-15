#include "lexer.h"
#include "tools.h"

t_token	*init_token_node(t_lexer *aut)
{
	t_token	*tk;

	tk = (t_token*)malloc(sizeof(t_token));
	tk->token = ft_strdup(aut->current_token);
	//if ((op = is_operator(tk->token)) != -1)
	//	tk->type = op;
	if (!ft_strcmp(aut->current_token, "\n"))
		tk->type = NEWLINE;
	else if (!ft_strcmp(aut->current_token, ">>"))
		tk->type = DGREAT;
	else if (!ft_strcmp(aut->current_token, "<<"))
		tk->type = DLESS;
	else if (!ft_strcmp(aut->current_token, "||"))
		tk->type = OR_IF;
	else if (!ft_strcmp(aut->current_token, "&&"))
		tk->type = AND_IF;
	else if (!ft_strcmp(aut->current_token, ">&"))
		tk->type = GREATAND;
	else if (!ft_strcmp(aut->current_token, "<&"))
		tk->type = LESSAND;
	else if (!ft_strcmp(aut->current_token, "|"))
		tk->type = '|';
	else if (!ft_strcmp(aut->current_token, ">"))
		tk->type = '>';
	else if (!ft_strcmp(aut->current_token, "<"))
		tk->type = '<';
	else if (!ft_strcmp(aut->current_token, ";"))
		tk->type = ';';
	else
		tk->type = WORD;
	ft_bzero(aut->current_token, aut->token_len);
	aut->token_len = 0;
	tk->next = NULL;
	return (tk);	
}

void	push_back_token(t_lexer *aut)
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
