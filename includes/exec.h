#ifndef EXEC_H
# define EXEC_H
# include "libft.h"
# include "parser.h"
# include "sys/wait.h"
# include "sys/types.h"
# include "builtins.h"

#define COMMAND_NOT_FOUND 127
#define PERMISSION_DENIED 126

int		redir_great(t_redir *redir);
int		redir_dgreat(t_redir *redir);
int		redir_less(t_redir *redir);
int		redir_greatand(t_redir *redir);
void	launch_command(t_exec *exec, t_env *env, char **cmd);
void	clear_exec(t_exec **exec);
char	**lst_to_tab(t_list *word_list, int word_count);

# endif
