/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:57:19 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/28 17:44:45 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSH_H
# define BSH_H

# include "input.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"
# include "history.h"
# include "builtins.h"
# include "expander.h"

# define INTERACTIVE 0
# define FILE_MODE 1

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
	t_expander	*exp;
	pid_t		pid;
	int			exit_status;
	char		*shell_name;
}				t_bsh;

t_bsh			*get_bsh(void);
void			clear_exec(t_exec **exec);
void			clear_token_list(t_token **token);

void			waiting_for_input(t_input *input);

int				ft_perm_denied_msg(char *file);
int				ft_no_file_msg(char *file);
int				ft_cmd_not_found(char *cmd);
int				check_access_exist(char *file);
int				check_access_writing(char *file);
int				check_access_reading(char *file);
#endif
