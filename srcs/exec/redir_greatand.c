#include "bsh.h"

int	redir_greatand(t_redir *redir)
{
	if (!ft_strcmp(redir->filename, "-"))
		close(redir->fd);
	else if (ft_str_isdigit(redir->filename))
	{
		dup2(redir->fd, ft_atoi(redir->filename));
		close(redir->fd);
	}
	return (0);
} 
