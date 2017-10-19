/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_stack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 20:52:19 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 20:45:24 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

extern t_switch	g_parser[MAX_STATES][MAX_EVENTS];

void			pop_stack(t_stack **stack)
{
	t_stack		*tmp;

	if (*stack)
	{
		tmp = *stack;
		*stack = (*stack)->next;
		tmp->next = NULL;
		ft_memdel((void**)&tmp);
	}
}

void			push_token_stack(t_parser *parser)
{
	t_stack		*node;

	if ((node = (t_stack*)malloc(sizeof(t_stack))) == NULL)
		exit(EXIT_FAILURE);
	node->sym.type = parser->cur_token->type;
	node->sym.value = parser->cur_token->token;
	node->state = -1;
	node->next = parser->stack;
	parser->stack = node;
}

void			push_state(t_parser *parser)
{
	t_stack		*node;
	int			state;

	if ((node = (t_stack*)malloc(sizeof(t_stack))) == NULL)
		exit(EXIT_FAILURE);
	state = g_parser[parser->state][parser->cur_token->type].transition;
	node->state = state;
	node->sym.type = -1;
	parser->state = state;
	node->next = parser->stack;
	parser->stack = node;
}
