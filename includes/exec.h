#ifndef EXEC_H
# define EXEC_H
# include "libft.h"
# include "parser.h"
# include "sys/wait.h"
# include "sys/types.h"

int	redir_great(t_exec *exec);
int	redir_dgreat(t_exec *exec);
int	redir_less(t_exec *exec);
void	launch_command(t_exec *exec);
# endif
