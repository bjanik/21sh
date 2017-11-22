/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:57:19 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/22 17:23:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSH_H
# define BSH_H

# include <signal.h>
# include <pwd.h>
# include <sys/types.h>
# include <uuid/uuid.h>
# include "libft.h"
# include "input.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"
# include "history.h"
# include "builtins.h"
# include "expander.h"

# define INTERACTIVE 0
# define FILE_MODE 1

# define REGULAR_INPUT 0
# define HEREDOC_INPUT 1

# define STOP_HEREDOC 1
# define UNCLOSED_QUOTES 2
# define UNEXPECTED_EOF 3
# define RETURN 4

pid_t	g_pid;
typedef struct	s_bsh
{
	t_input		*input;
	t_term		*term;
	t_lexer		*lexer;
	t_parser	*parser;
	t_exec		*exec;
	t_env		*env;
	t_env		*mod_env;
	t_history	*history;
	t_token		*tokens[2];
	t_expander	*exp;
	pid_t		pid;
	t_pipes		*pipes;
	int			exit_status;
	char		*shell_name;
	int			saved_fds[3];
}				t_bsh;

t_bsh			*get_bsh(void);
t_bsh			*shell_init(char **environ);
void			clear_exec(t_exec **exec);
void			clear_token_list(t_token **token);
void			del(void *content, size_t size);
int				handle_unclosed_quotes(t_lexer *lexer, t_input *input, int *ret,
				t_token *tokens[]);

int				wait_for_input(t_input *input, int input_type);
void			execution(t_bsh *bsh);

int				ft_perm_denied_msg(char *file);
int				ft_no_file_msg(char *file);
int				ft_cmd_not_found(char *cmd);
int				check_access_exist(char *file);
int				check_access_writing(char *file);
int				check_access_reading(char *file);

void			set_signals(void);
void			winsize_change(int signum);
void			sigint_handler(int signum);

void			save_history_to_hist_file(void);
#endif
