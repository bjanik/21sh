#include "parser.h"

t_exec	*init_exec(void)
{
	t_exec	*exec;

	if (!(exec = (t_exec*)malloc(sizeof(t_exec))))
		return (NULL);
	exec->word_list = NULL;
	exec->last_word = NULL;
	exec->word_count = 0;
	exec->redir_list = NULL;
	exec->last_redir = NULL;
	exec->cmd_separator = 0;
	exec->exit_status = 0;
	exec->next = NULL;
	exec->prev = NULL;
	return (exec);
}
