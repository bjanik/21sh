#include "libft.h"
#include "lexer.h"
#include "expander.h"
#include "tools.h"
#include "history.h"
#include "input.h"

int             init_termcaps(void)
{
        char                    *termtype;
        struct termios  term;

        if (!isatty(STDIN))
                exit(-1);
        if (!(termtype = getenv("TERM")))
                ft_error_msg("Missing $TERM variable");
        if (!tgetent(NULL, termtype))
                exit(-1);
        if (tcgetattr(STDIN, &term) == -1)
                exit(-1);
        term.c_lflag &= ~(ICANON | ECHO);
        term.c_cc[VMIN] = 1;
        term.c_cc[VTIME] = 0;
        if (tcsetattr(STDIN, TCSADRAIN, &term) == -1)
                ft_error_msg("Unable to set terminal");
        return (0);
}


int	main(int argc, char **argv, char **env)
{
	t_token		*token_lst;
	t_history	*history;
	t_input		*input;
	t_term		*term;

	term = init_term();
        history = init_history();
        input = init_input(term, history);
        init_termcaps();
	while (42)
	{
		ft_bzero(input->buffer, input->buffer_len);
		input->buffer_len = 0;
		term->print_prompt(term, BOLD_CYAN);
		while (42)
                {
                        ft_bzero(input->read_buffer, MAX_KEY_LENGTH);
                        if (read(STDIN, input->read_buffer, MAX_KEY_LENGTH) < 1)
                                exit(-1);
                        if (input->get_key(input))
                                break ;
		}
		write(STDOUT, RETURN, 1);
		token_lst = tokenizer(input->buffer);
		while (token_lst)
		{
			dprintf(input->fd, "[%s]\n", token_lst->token);
			token_lst = token_lst->next;
		}
	}
	//g_token_lst = token_lst;
	//yyparse();
}

