#ifndef PARSER_H
# define PARSER_H
# include "libft.h"
# include "tools.h"
# define MAX_STATES 51
# define MAX_EVENTS 14
# define NB_RULES 39

typedef enum e_sym {
			ACCEPT,
			PROGRAM,
			COMPLETE_COMMAND,
			LIST,
			AND_OR,
			PIPELINE,
			COMMAND,
			CMD_PREFIX,
			CMD_NAME,
			CMD_SUFFIX,
			CMD_WORD,
			IO_REDIRECT,
			IO_FILE,
			IO_HERE,
			FILENAME,
			HERE_END,
			SEPARATOR_OP,
			MAX_SYM,
		}e_sym;


typedef struct s_sym
{
	int	type;
	char	*value;
}		t_sym;

typedef struct		s_stack
{
	t_sym		sym;
	int		state;
	struct s_stack	*next;
}			t_stack;

typedef struct s_parser
{
	t_stack		*stack;
	int		state;
	t_token		*cur_token;
}			t_parser;

typedef struct 		s_switch
{
	int		rule_number;
	int		(*p_switch)(t_parser *parser);
	int		new_state;
}			t_switch;

int		shift(t_parser *parser);
int		reduce(t_parser *parser);
int		syntax_error(t_parser *parser);
int		accept(t_parser *parser);
void		push__token_stack(t_parser *parser);
void		push_state(t_parser *parser);
void		pop_stack(t_stack **stack);
t_parser	*init_parser(t_token *token_list);

#endif
