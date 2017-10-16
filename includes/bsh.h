#ifndef BSH_H
# define BSH_H

# include "input.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"
# include "history.h"
# include "builtins.h"

typedef struct	s_bsh
{
	t_input		*input;
	t_term		*term;
	t_lexer		*lexer;
	t_parser	*parser;
	t_exec		*exec;
	t_env		*env;
	t_history	*history;
	t_token		*tokens[2];
}				t_bsh;

t_bsh			*get_bsh(void);

#endif
