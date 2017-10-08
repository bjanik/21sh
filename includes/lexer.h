#ifndef LEXER_H
# define LEXER_H
# include "libft.h"
# include "tools.h"

#define INITIAL_TOKEN_SIZE 512

t_token	*g_token_lst;

typedef enum e_state {
			INIT,
			DQUOTE,
			QUOTE,
			BQUOTE,
			STD,
			NWLINE,
			COMMENT,
			MAX_STATE,
		     } 	e_state;

typedef enum e_event {
			START,
			EV_DQUOTE,
			EV_QUOTE,
			EV_BQUOTE,
			EV_OP_CHAR,
			EV_BLANK,
			EV_NEWLINE,
			EV_COMMENT,
			EV_REG_CHAR,
			EV_BACKSLASH,
			MAX_EVENT} e_event;

typedef struct s_lexer
{
	char 	*input;
	char	*current_token;
	int	token_len;
	int	token_size;
	e_state	state;
	size_t	event;
	t_token	*token_list;
	t_token	*last_token;
}		t_lexer;

typedef struct	s_transition
{
	e_state	new_state;
	void 	(*p_transition)(t_lexer *lexer);
}		t_transition;

t_lexer		*lexer(t_lexer *lexer, char *input, int initial_state);
t_token		*init_token_node(t_lexer *lexer);
void		skip_char(t_lexer *lexer);
void		push_back_token(t_lexer *lexer);
void		append_char(t_lexer *lexer);
void		delimitate_token(t_lexer *lexer);
void		handle_backslash(t_lexer *lexer);
void		end_of_input(t_lexer *lexer);
void		get_operator(t_lexer *lexer);
void		init(t_lexer *lexer);

extern t_transition	g_lexer[MAX_STATE][MAX_EVENT];
extern char	g_op_char[6];
extern char	*g_op_list[12];
#endif
