#include "parser.h"

int	redir_great(t_exec *exec)
{
	int	fd;

	if ((fd = open(exec->redir_list->dest_file, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
		return (0);
	dup2(fd, exec->redir_list->fd);
	close(fd);
	return (1);
}

int	redir_dgreat(t_exec *exec)
{
	int	fd;

	if ((fd = open(exec->redir_list->dest_file, O_CREAT | O_WRONLY | O_APPEND, 0644)) < 0)
		return (0);
	dup2(fd, exec->redir_list->fd);
	close(fd);
	return (1);
}

int	redir_less(t_exec *exec)
{
	int	fd;

	if ((fd = open(exec->redir_list->dest_file, O_RDONLY, 0644)) < 0)
		return (0);
	dup2(fd, STDIN);
	close(fd);
	return (1);
}