/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:39:37 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/11 17:26:05 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer		*init_lexer(t_lexer *lexer, char *input, int initial_state)
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
