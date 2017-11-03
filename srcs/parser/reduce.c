/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:34:28 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/03 15:56:28 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

extern t_save_exec	g_save_exec_list[NB_RULES];
extern int			g_reduce_table[NB_RULES];
extern t_switch		g_parser[MAX_STATES][MAX_EVENTS];
extern int			g_rules_len[NB_RULES];

int		reduce(t_parser *parser)
{
	int	i;
	int	k;

	i = g_parser[parser->state][parser->cur_token->type].transition;
	if (g_save_exec_list[i].save && parser->exec_list)
		g_save_exec_list[i].save(parser->last_exec, parser->stack);
	k = i;
	i = g_rules_len[i] * 2 - 1;
	while (i--)
		pop_stack(&parser->stack);
	parser->stack->sym.type = g_reduce_table[k];
	if (parser->stack->next)
		parser->state = parser->stack->next->state;
	get_successor_state(parser, parser->stack->sym.type);
	return (-2);
}
