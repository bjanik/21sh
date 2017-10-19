#include "bsh.h"

int	redir_great(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (!check_access_exist(redir->dest_file) &&
			check_access_writing(redir->dest_file))
		return (1);
	if ((fd = open(redir->dest_file, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
		return (1);
	dup2(fd, redir->fd);
	close(fd);
	return (0);
}

int	redir_dgreat(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (!check_access_exist(redir->dest_file) &&
			check_access_writing(redir->dest_file))
		return (1);
	if ((fd = open(redir->dest_file, O_CREAT | O_WRONLY | O_APPEND, 0644)) < 0)
		return (1);
	dup2(fd, redir->fd);
	close(fd);
	return (0);
}

int	redir_less(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (check_access_exist(redir->dest_file) ||
			check_access_reading(redir->dest_file))
		return (1);
	if ((fd = open(redir->dest_file, O_RDONLY, 0644)) < 0)
		return (1);
	dup2(fd, STDIN);
	close(fd);
	return (0);
}
