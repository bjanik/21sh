#include "libft.h"

typedef struct 		s_token
{
	char		*token;
	struct s_token	*next;
}			t_token;

typedef enum e_state {
			INIT,
			DQUOTE,
			QUOTE,
			BQUOTE,
			BACKSLASH,
			STD,
			NEWLINE,
			COMMENT,
			MAX_STATE,
			PREVIOUS} e_state;

typedef enum e_event {
			START,
			EV_DQUOTE,
			EV_QUOTE,
			EV_BQUOTE,
			EV_BACKSLASH,
			EV_OP_CHAR,
			EV_BLANK,
			EV_NEWLINE,
			EV_COMMENT,
			EV_REG_CHAR,
			MAX_EVENT} e_event;


typedef struct 	s_event
{
	e_event	code;
	void	*data;
}		t_event;

typedef struct 	s_tokenizer
{
	char	*input;
	char	*current_token;
	int	token_len;
	e_state	state;
	e_state	prev_state;
	t_event	event;
	t_token	*token_list;
	void	(*append_char)(struct s_tokenizer *aut);
	void	(*delimitate_token)(struct s_tokenizer *aut);
	void	(*push_back_token)(struct s_tokenizer *aut);
	t_token	*(*init_token_node)(struct s_tokenizer *aut);
	
}		t_tokenizer;

typedef struct	s_transition
{
	e_state	new_state;
	void 	(*p_transition)(t_tokenizer *aut);
}		t_transition;

t_token		*init_token_node(t_tokenizer *aut);
void		push_back_token(t_tokenizer *aut);
void		append_char(t_tokenizer *aut);
void		delimitate_token(t_tokenizer *aut);
void		get_operator(t_tokenizer *aut);
void		init(t_tokenizer *aut);


extern t_transition	g_aut_token[MAX_STATE][MAX_EVENT];
extern char	g_op_char[6];
extern char	*g_op_list[11];
