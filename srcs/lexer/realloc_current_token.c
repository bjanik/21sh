/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_current_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:22:35 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 20:57:51 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		realloc_current_token(t_lexer *lexer)
{
	char	*tmp;

	tmp = lexer->current_token;
	lexer->token_size *= 2;
	if (!(lexer->current_token = (char *)ft_memalloc((lexer->token_size * 2 + 1)
					* sizeof(char))))
		exit(EXIT_FAILURE);
	ft_bzero(lexer->current_token, lexer->token_size + 1);
	ft_strcpy(lexer->current_token, tmp);
	free(tmp);
}
