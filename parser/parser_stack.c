#include "parser.h"

extern t_switch g_aut_parser[MAX_STATES][MAX_EVENTS];

void		pop_stack(t_stack **stack)
{
	t_stack	*tmp;

	if (*stack)
	{
		tmp = *stack;
		*stack = (*stack)->next;
		tmp->next = NULL;
		free(tmp);		
	}	
}

void		push_token_stack(t_parser *parser)
{
	t_stack	*node;

	if ((node = (t_stack*)malloc(sizeof(t_stack))) == NULL)
		return ;
	node->sym.type = parser->cur_token->type;
	node->state = -1;
	node->next = parser->stack;
	parser->stack = node;
}

void		push_state(t_parser *parser)
{
	t_stack	*node;
	int	state;

	if ((node = (t_stack*)malloc(sizeof(t_stack))) == NULL)
		return ;
	state = g_aut_parser[parser->state][parser->cur_token->type].new_state;
	node->state = state;
	node->sym.type = -1;
	parser->state = state;
	node->next = parser->stack;
	parser->stack = node;

}

t_parser	*init_parser(t_token *token_list)
{
	t_parser	*parser;

	if (!(parser = (t_parser*)malloc(sizeof(t_parser))))
		return (NULL);
	if (!(parser->stack = (t_stack*)malloc(sizeof(t_stack))))
		return (NULL);
	parser->stack->next = NULL;
	parser->stack->state = 0;
	parser->stack->sym.type = -1;
	parser->state = 0;
	parser->cur_token = token_list;
	return (parser);
}
