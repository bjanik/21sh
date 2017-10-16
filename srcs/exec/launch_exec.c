#include "exec.h"
#include "parser.h"

char		**lst_to_tab(t_list *word_list, int word_count)
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

void		handle_redirection(t_exec *exec)
{
	t_redir	*redir;
	
	redir = exec->redir_list;
	while (redir)
	{
		if (redir->type == GREAT)
			redir_great(redir);
		else if (redir->type == DGREAT)
			redir_dgreat(redir);
		else if (redir->type == LESS)
			redir_less(redir);
		else if (redir->type == GREATAND)
			redir_greatand(redir);
		redir = redir->next;
	}
}

char		**get_cmd_path(t_env *env)
{
	char	**paths;
	t_env	*path;

	if (!(path = ft_getenv(env, "PATH")))
		return (NULL);
	paths = ft_strsplit(path->var_value, ':');
	return (paths);
}

void		launch_command(t_exec *exec, t_env *env, char **cmd)
{
	int		pid;
	int		i;
	char	**paths;
	char	*full_path;
	char	**env_tab;

	i = -1;
	if ((pid = fork()) < 0)
		exit(-1);
	if (!pid)
	{
		handle_redirection(exec);
		paths = get_cmd_path(env);
		env_tab = env_to_tab(env);
		while (paths && paths[++i] && cmd[0][0] != '/')
		{
			full_path = ft_strnjoin(paths[i], 2, "/", cmd[0]);
			cmd[0] = ft_strdup(exec->word_list->content);
			if (access(full_path, F_OK | X_OK) == 0)
				execve(full_path, cmd, env_tab);
		}
		if (access(cmd[0], F_OK | X_OK) == 0)
			execve(cmd[0], cmd, env_tab);
		ft_printf("bsh: %s: command not found...\n", cmd[0]);
		exit(COMMAND_NOT_FOUND);
	}
	else
	{
		waitpid(pid, &exec->exit_status, 0);
		if (WIFEXITED(exec->exit_status))
			exec->exit_status = WEXITSTATUS(exec->exit_status);
	}
}
