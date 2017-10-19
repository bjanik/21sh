/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:47:01 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 20:39:39 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser	*init_parser(t_token *token_list, int ex)
{
	t_parser	*parser;

	if (!(parser = (t_parser*)malloc(sizeof(t_parser))))
		exit(EXIT_FAILURE);
	if (!(parser->stack = (t_stack*)malloc(sizeof(t_stack))))
		exit(EXIT_FAILURE);
	parser->stack->next = NULL;
	parser->stack->state = 0;
	parser->stack->sym.type = -1;
	parser->stack->sym.value = NULL;
	parser->state = 0;
	parser->cur_token = token_list;
	parser->exec_list = (ex == SAVE_EXEC) ? init_exec(): NULL;
	parser->last_exec = parser->exec_list;
	return (parser);
}

