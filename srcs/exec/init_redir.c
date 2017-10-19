#include "bsh.h"

t_redir	*init_redir(void)
{
	t_redir	*redir;

	if (!(redir = (t_redir*)malloc(sizeof(t_redir))))
		exit(EXIT_FAILURE);
	redir->next = NULL;
	redir->dest_file = NULL;
	redir->here_end = NULL;
	redir->fd = STDOUT;
	return (redir);
}
