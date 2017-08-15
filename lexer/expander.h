#ifndef EXPANDER_H
# define EXPANDER_H
# include "tools.h"
# include "history.h"
# define INITIAL_SIZE 256

typedef enum e_state1 {
			INIT1,
			STD1,
			DQUOTE1,
			QUOTE1,
			MAX_STATE1,
			} e_state1;

typedef enum e_event1 {
			START1,
			EV_DQUOTE1,
			EV_QUOTE1,
			EV_DOLLAR,
			EV_BANG,
			EV_TILDE,
			EV_BACKSLASH,
			EV_REG_CHAR1,
			MAX_EVENT1,
			}e_event1;

typedef struct	s_expander	
{
	e_state1		state;
	int		event;
	char		**env;
	t_history	*history;
	t_token		*token_lst;
	char		*buffer;
	int		buffer_len;
	char		*tmp;
	void		(*append)(struct s_expander *exp);
	void		(*handle_tilde)(struct s_expander *exp);
	void		(*handle_backslash)(struct s_expander *exp);
	void		(*handle_dollar)(struct s_expander *exp);

}		t_expander;

typedef struct	s_transit
{
	e_state1	new_state;
	void 	(*p_transit)(t_expander *exp);
}		t_transit;


void		append(struct s_expander *exp);
void		skip(struct s_expander *exp);
void		handle_tilde(struct s_expander *exp);
void		handle_dollar(struct s_expander *exp);
void		handle_backslash(struct s_expander *exp);
void		handle_disclaimer(struct s_expander *exp);

extern t_transit	g_aut_expander[MAX_STATE1][MAX_EVENT1];
#endif
