#include "parser.h"

t_redir	*init_redir(t_exec *exec, t_stack *stack)
{
	t_redir	*redir;

	if (!(redir = (t_redir*)malloc(sizeof(t_redir))))
		return (NULL);
	redir->next = NULL;
	redir->dest_file = NULL;
	redir->here_end = NULL;
	redir->fd = STDOUT;
	return (redir);
}
