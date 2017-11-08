/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 15:39:37 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/08 14:47:03 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_lexer		*init_lexer(char *input, int initial_state)
{
	t_lexer	*lexer;

	if (!(lexer = (t_lexer*)malloc(sizeof(t_lexer))))
		exit(EXIT_FAILURE);
	if (!(lexer->current_token = (char*)malloc(INITIAL_TOKEN_SIZE + 1)))
		exit(EXIT_FAILURE);
	lexer->input = input;
	ft_bzero(lexer->current_token, INITIAL_TOKEN_SIZE + 1);
	lexer->token_size = INITIAL_TOKEN_SIZE;
	lexer->token_len = 0;
	lexer->state = initial_state;
	lexer->event = START;
	lexer->token_list[0] = NULL;
	lexer->token_list[1] = NULL;
	return (lexer);
}
