#include "lexer.h"
#include "tools.h"

t_token	*init_token_node(t_lexer *lexer)
{
	t_token	*token;

	token = (t_token*)malloc(sizeof(t_token));
	token->token = ft_strdup(lexer->current_token);
	token->pushed = 0;
	//if ((op = is_operator(token->token)) != -1)
	//	token->type = op;
	if (!ft_strcmp(lexer->current_token, "\n") && (lexer->state == STD ||
			lexer->state == COMMENT))
		token->type = NEWLINE;
	else if (!ft_strcmp(lexer->current_token, ">>"))
		token->type = DGREAT;
	else if (!ft_strcmp(lexer->current_token, "<<"))
		token->type = DLESS;
	else if (!ft_strcmp(lexer->current_token, "||"))
		token->type = OR_IF;
	else if (!ft_strcmp(lexer->current_token, "&&"))
		token->type = AND_IF;
	else if (!ft_strcmp(lexer->current_token, ">&"))
		token->type = GREATAND;
	else if (!ft_strcmp(lexer->current_token, "<&"))
		token->type = LESSAND;
	else if (!ft_strcmp(lexer->current_token, "|"))
		token->type = PIPE;
	else if (!ft_strcmp(lexer->current_token, ">"))
		token->type = GREAT;
	else if (!ft_strcmp(lexer->current_token, "<"))
		token->type = LESS;
	else if (!ft_strcmp(lexer->current_token, ";"))
		token->type = SEMI;
	else if (!ft_strcmp(lexer->current_token, "&"))
		token->type = AND;
	else
		token->type = WORD;
	token->next = NULL;
	ft_bzero(lexer->current_token, lexer->token_len);
	lexer->token_len = 0;
	return (token);	
}
