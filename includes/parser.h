#ifndef PARSER_H
# define PARSER_H
# include "libft.h"
# include "tools.h"
# include "exec.h"
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

typedef struct		s_redir
{
	char		*dest_file;
	int		fd;
	char		*here_end;
	int		type;
	struct s_redir	*next;
}			t_redir;

typedef struct 		s_exec
{
	t_list		*word_list;
	int		word_count;
	t_list		*last_word;
	t_redir		*redir_list;
	t_redir		*last_redir;
	int		cmd_separator;
	int		exit_status;
	struct s_exec	*next;
	struct s_exec	*prev;
}			t_exec;

typedef struct 		s_sym
{
	int		type;
	char		*value;
}			t_sym;

typedef struct		s_stack
{
	t_sym		sym;
	int		state;
	struct s_stack	*next;
}			t_stack;

typedef struct 		s_parser
{
	t_stack		*stack;
	int		state;
	t_token		*cur_token;
	t_exec		*exec_list;
	t_exec		*last_exec;
}			t_parser;

typedef struct 		s_switch
{
	int		transition;
	int		(*p_switch)(t_parser *parser);
}			t_switch;

typedef struct		s_save_exec
{
	void		(*save)(t_exec *exec_list, t_stack *stack);
}			t_save_exec;

int		shift(t_parser *parser);
int		reduce(t_parser *parser);
int		syntax_error(t_parser *parser);
int		accept(t_parser *parser);
void		push_token_stack(t_parser *parser);
void		push_state(t_parser *parser);
void		pop_stack(t_stack **stack);
void		parser(t_token *list);
t_parser	*init_parser(t_token *token_list);

t_exec		*init_exec(void);
t_redir		*init_redir(t_exec *exec, t_stack *stack);
void		append_wordlist(t_exec *exec, t_stack *stack);
void		set_io_number(t_exec *exec, t_stack *stack);
void		set_here_end(t_exec *exec, t_stack *stack);
void		set_dest_file(t_exec *exec, t_stack *stack);

#endif
