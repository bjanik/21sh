/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimitate_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:13:50 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/11 16:54:04 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
