/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:57:29 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/02 12:38:38 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "libft.h"
# include "parser.h"
# include "builtins.h"

# define COMMAND_NOT_FOUND 127
# define PERMISSION_DENIED 126
# define READ 0
# define WRITE 1

typedef struct		s_redir
{
	char			*filename;
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
	char			**cmd;
	t_redir			*redir_list;
	t_redir			*last_redir;
	int				is_builtin;
	int				cmd_separator;
	int				exit_status;
	struct s_exec	*next;
	struct s_exec	*prev;
}					t_exec;

int					redir_great(t_redir *redir);
int					redir_dgreat(t_redir *redir);
int					redir_less(t_redir *redir);
int					redir_greatand(t_redir *redir);
int					redir_lessand(t_redir *redir);
int					handle_redirection(t_exec *exec);

char				**get_cmd_path(t_env *env);
char				**lst_to_tab(t_list *word_list, int word_count);
void				run_binary(t_exec *exec, t_env *env);
void				run_builtin(int builtin, char **cmd);
int					**get_pipes_fd(t_exec *exec, int *nb_pipes);
void				pipe_sequence(t_exec **exec, int **pipes_fd, int nb_pipes);

void				clear_exec(t_exec **exec);
# endif
