/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:23:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/03 15:23:43 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_token		*init_token_node(t_lexer *lexer)
{
	t_token	*token;
	int		op;

	op = 0;
	if (!(token = (t_token*)malloc(sizeof(t_token))))
		exit(EXIT_FAILURE);
	if (!(token->token = ft_strdup(lexer->current_token)))
		exit(EXIT_FAILURE);
	token->pushed = 0;
	if ((op = is_operator(token->token)) != -1)
		token->type = op;
	else if (!ft_strcmp(lexer->current_token, "\n") && (lexer->state == STD ||
			lexer->state == COMMENT))
		token->type = NEWLINE;
	else
		token->type = WORD;
	token->next = NULL;
	ft_bzero(lexer->current_token, lexer->token_len);
	lexer->token_len = 0;
	return (token);
}
