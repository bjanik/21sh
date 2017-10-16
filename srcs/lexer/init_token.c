#include "lexer.h"
#include "tools.h"

t_token	*init_token_node(t_lexer *lexer)
{
	t_token	*token;
	int		op;

	op = 0;
	if (!(token = (t_token*)malloc(sizeof(t_token))))
		return (NULL);
	if (!(token->token = ft_strdup(lexer->current_token)))
		return (NULL);
	token->pushed = 0;
	if ((op = is_operator(token->token)) != -1)
		token->type = op;
	else if (!ft_strcmp(lexer->current_token, "\n") && (lexer->state == STD ||
			lexer->state == COMMENT))
		token->type = NEWLINE;
	else
		token->type = WORD;
	/*else if (!ft_strcmp(lexer->current_token, ">>"))
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
		token->type = AND;*/
	token->next = NULL;
	ft_bzero(lexer->current_token, lexer->token_len);
	lexer->token_len = 0;
	return (token);
}
