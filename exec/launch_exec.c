#include "exec.h"
#include "parser.h"

char	**lst_to_tab(t_list *word_list, int word_count)
{
	char	**tab;
	int	i;

	i = 0;
	if (!(tab = (char**)malloc(sizeof(char*) * (word_count + 1))))
		return (NULL);
	while (word_list)
	{
		tab[i++] = word_list->content;
		word_list = word_list->next;
	}
	tab[i] = NULL;
	return (tab);
}


void	handle_redirection(t_exec *exec)
{
	t_redir	*list;
	
	list = exec->redir_list;
	while (list)
	{
		ft_printf("%p - file = %s - type = %d - here = %s\n", list, list->dest_file, list->type, list->here_end);
		if (list->type == GREAT)
			redir_great(exec);
		else if (list->type == DGREAT)
			redir_dgreat(exec);
		else if (list->type == LESS)
			redir_less(exec);
		list = list->next;
	}	
}

void	launch_command(t_exec *exec)
{
	int	pid;
	char	**cmd;
	
	if ((pid = fork()) < 0)
		exit(-1);
	if (!pid)
	{
		if (exec->word_list)
			cmd = lst_to_tab(exec->word_list, exec->word_count);
		handle_redirection(exec);
		execve(cmd[0], cmd, NULL);
	}
	else
	{
		waitpid(pid, &exec->exit_status, 0);
		if (WIFEXITED(exec->exit_status))
			exec->exit_status = WEXITSTATUS(exec->exit_status);
	}
}
