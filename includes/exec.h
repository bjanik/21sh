/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:57:29 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 17:57:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "libft.h"
# include "parser.h"
# include "sys/wait.h"
# include "sys/types.h"
# include "builtins.h"

# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define IS_BUILTIN 0
# define IS_BINARY 1

typedef struct		s_redir
{
	char			*dest_file;
	char			*here_end;
	int				fd;
	int				type;
	struct s_redir	*next;
}					t_redir;

typedef struct		s_exec
{
	t_list			*word_list;
	t_list			*last_word;
	int				word_count;
	t_redir			*redir_list;
	t_redir			*last_redir;
	int				cmd_separator;
	int				exit_status;
	struct s_exec	*next;
	struct s_exec	*prev;
}					t_exec;

int					redir_great(t_redir *redir);
int					redir_dgreat(t_redir *redir);
int					redir_less(t_redir *redir);
int					redir_greatand(t_redir *redir);
int					handle_redirection(t_exec *exec);
void				launch_command(t_exec *exec, t_env *env, char **cmd);
void				clear_exec(t_exec **exec);
char				**lst_to_tab(t_list *word_list, int word_count);
# endif
