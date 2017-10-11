#include "parser.h"
#include "exec.h"

int	redir_greatand(t_redir *redir)
{
	//ft_printf("IN >&, file  = %s && fd = %d\n", redir->dest_file, redir->fd);
	if (!ft_strcmp(redir->dest_file, "-"))
		close(redir->fd);
	else if (ft_str_isdigit(redir->dest_file))
	{
		dup2(redir->fd, ft_atoi(redir->dest_file));
		close(redir->fd);
	}
	return (0);
} 
