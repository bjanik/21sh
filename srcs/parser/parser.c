/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 16:09:03 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 16:12:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

extern int			g_successor_state[18][MAX_SYM];
extern int			g_reduce_table[NB_RULES];
extern t_save_exec	g_save_exec_list[NB_RULES];

const t_switch		g_parser[MAX_STATES][MAX_EVENTS] = {
	{{1, shift},
		{2, shift},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce},
		{17, reduce}},

	{{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce},
		{2, reduce}},

	{{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{23, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{25, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{25, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{25, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{25, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{25, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept},
		{-1, accept}},

	{{-1, syntax_error},
		{31, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{-1, NULL},
		{4, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{33, shift},
		{32, shift},
		{-1, NULL}},

	{{-1, NULL},
		{6, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{35, shift},
		{36, shift},
		{6, reduce},
		{6, reduce},
		{-1, NULL}},

	{{-1, NULL},
		{7, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{7, reduce},
		{7, reduce},
		{7, reduce},
		{37, shift},
		{37, shift}},

	{{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce},
		{10, reduce}},

	{{38, shift},
		{14, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{14, reduce},
		{14, reduce},
		{14, reduce},
		{14, reduce},
		{14, reduce}},

	{{41, shift},
		{16, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{16, reduce},
		{16, reduce},
		{16, reduce},
		{16, reduce},
		{16, reduce}},

	{{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce},
		{19, reduce}},

	{{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce},
		{25, reduce}},

	{{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce},
		{27, reduce}},

	{{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce},
		{26, reduce}},

	{{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce},
		{28, reduce}},

	{{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce},
		{36, reduce}},

	{{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce},
		{35, reduce}},

	{{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce},
		{34, reduce}},

	{{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce},
		{33, reduce}},

	{{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce},
		{30, reduce}},

	{{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce},
		{32, reduce}},

	{{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce},
		{29, reduce}},

	{{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce},
		{31, reduce}},

	{{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce},
		{1, reduce}},

	{{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce},
		{37, reduce}},

	{{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce},
		{38, reduce}},

	{{1, shift},
		{3, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{1, shift},
		{-1, syntax_error},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{1, shift},
		{-1, syntax_error},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{1, shift},
		{-1, syntax_error},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error},
		{-1, syntax_error}},

	{{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce},
		{18, reduce}},

	{{41, shift},
		{13, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{13, reduce},
		{13, reduce},
		{13, reduce},
		{13, reduce},
		{13, reduce}},

	{{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce},
		{20, reduce}},

	{{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce},
		{23, reduce}},

	{{49, shift},
		{15, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{15, reduce},
		{15, reduce},
		{15, reduce},
		{15, reduce},
		{15, reduce}},

	{{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce},
		{21, reduce}},

	{{-1, NULL},
		{5, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{35, shift},
		{36, shift},
		{5, reduce},
		{5, reduce},
		{-1, NULL}},

	{{-1, NULL},
		{8, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{8, reduce},
		{8, reduce},
		{8, reduce},
		{8, reduce},
		{37, shift}},

	{{-1, NULL},
		{9, reduce},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{-1, NULL},
		{9, reduce},
		{9, reduce},
		{9, reduce},
		{9, reduce},
		{37, shift}},

	{{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce},
		{11, reduce}},

	{{49, shift},
		{12, reduce},
		{3, shift},
		{4, shift},
		{5, shift},
		{6, shift},
		{7, shift},
		{8, shift},
		{9, shift},
		{12, reduce},
		{12, reduce},
		{12, reduce},
		{12, reduce},
		{12, reduce}},

	{{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce},
		{24, reduce}},

	{{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce},
		{22, reduce}},
};

static const int	g_rules_len[] = {
	2,
	2,
	1,
	2,
	1,
	3,
	1,
	1,
	3,
	3,
	1,
	3,
	3,
	2,
	1,
	2,
	1,
	1,
	1,
	1,
	2,
	1,
	2,
	1,
	2,
	1,
	2,
	1,
	2,
	2,
	2,
	2,
	2,
	2,
	1,
	2,
	1,
	1,
	1};

int					syntax_error(t_parser *parser)
{
	int				type;

	type = parser->cur_token->type;
	if (parser->state == 0 || (parser->state > 2 && parser->state < 10) ||
			(parser->state >= 34 &&
			parser->state <= 37 && type != NEWLINE) ||
			parser->state == 11)
		if (type == NEWLINE)
			ft_printf("bsh: syntax error near unexpected token `newline'\n");
		else
			ft_printf("bash: syntax error near unexpected token `%s'\n",
						parser->cur_token->token);
	else
		return (END_IS_OP);
	return (SYNTAX_ERROR);
}

int					accept(t_parser *parser)
{
	(void)parser;
	return (ACCEPTED);
}

int					shift(t_parser *parser)
{
	if (parser->exec_list && (parser->cur_token->type >= AND_IF &&
		parser->cur_token->type <= PIPE))
	{
		parser->last_exec->cmd_separator = parser->cur_token->type;
		parser->last_exec->next = init_exec();
		parser->last_exec->next->prev = parser->last_exec;
		parser->last_exec = parser->last_exec->next;
	}
	if (parser->cur_token->pushed == 1)
		return (UNCLOSED_QUOTES);
	push_token_stack(parser);
	push_state(parser);
	parser->cur_token->pushed = 1;
	if (parser->cur_token->next)
	{
		parser->cur_token->pushed = 0;
		parser->cur_token = parser->cur_token->next;
	}
	return (-1);
}

int					get_successor_state(t_parser *parser, int sym_type)
{
	int				i;
	t_stack			*node;

	i = -1;
	while (++i < MAX_SYM)
	{
		if (g_successor_state[i][0] == parser->state)
			break ;
	}
	parser->state = g_successor_state[i][sym_type];
	if ((node = (t_stack*)malloc(sizeof(t_stack))) == NULL)
		return (-1);
	node->state = parser->state;
	node->sym.type = -1;
	node->sym.value = NULL;
	node->next = parser->stack;
	parser->stack = node;
	return (0);
}

int					reduce(t_parser *parser)
{
	int				i;
	int				k;

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

void				display_prompt(t_input *input)
{
	input->term->prompt_len = 2;
	ft_bzero(input->buffer, input->buffer_len);
	ft_strcpy(input->term->prompt, "> ");
	write(STDOUT, "> ", 2);
	input->cursor_pos = 0;
	input->buffer_len = 0;
	input->term->cursor_col = 3;
}

int					parser(t_exec **exec, t_token *token_list, int ex)
{
	t_parser		*parse;
	int				ret;

	ret = -1;
	parse = init_parser(token_list, ex);
	while (ret < 0)
		ret = g_parser[parse->state][parse->cur_token->type].p_switch(parse);
	while (parse->stack)
		pop_stack(&parse->stack);
	if (ex && ret == ACCEPTED)
		*exec = parse->exec_list;
	else
		clear_exec(exec);
	ft_memdel((void**)&parse);
	return (ret);
}
