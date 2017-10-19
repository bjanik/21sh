#include "bsh.h"

char		**lst_to_tab(t_list *word_list, int word_count)
{
	t_list	*wd;
	char	**tab;
	int		i;

	i = 0;
	wd = word_list;
	if (!(tab = (char**)malloc(sizeof(char*) * (word_count + 1))))
		return (NULL);
	while (wd)
	{
		tab[i++] = wd->content;
		wd = wd->next;
	}
	tab[i] = NULL;
	return (tab);
}

int			handle_redirection(t_exec *exec)
{
	t_redir	*redir;
	int		ret;

	ret = 0;
	redir = exec->redir_list;
	while (redir && !ret)
	{
		if (redir->type == GREAT)
			ret = redir_great(redir);
		else if (redir->type == DGREAT)
			ret = redir_dgreat(redir);
		else if (redir->type == LESS)
			ret = redir_less(redir);
		else if (redir->type == GREATAND)
			ret = redir_greatand(redir);
		redir = redir->next;
	}
	return (ret);
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

int			access_exec_binary(char *bin_path)
{
	if (access(bin_path, F_OK))
		return (COMMAND_NOT_FOUND);
	else if (access(bin_path, X_OK))
	{
		ft_printf("ACCESS PERMISSION DENIED\n");
		return (PERMISSION_DENIED);
	}
	return (0);
}

void		launch_command(t_exec *exec, t_env *env, char **cmd)
{
	int		pid;
	int		i;
	char	**paths;
	char	*full_path;
	char	**env_tab;
	int		ret;
	char	*pwd;

	i = -1;
	ret = 0;
	if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (!pid)
	{
		(handle_redirection(exec)) ? exit(EXIT_FAILURE) : 0;
		paths = get_cmd_path(env);
		env_tab = env_to_tab(env);
		while (paths && paths[++i] && cmd[0][0] != '/' &&
				ft_strncmp("./", cmd[0], 2))
		{
			full_path = ft_strnjoin(paths[i], 2, "/", cmd[0]);
			cmd[0] = ft_strdup(exec->word_list->content);
			if (!(ret = access_exec_binary(full_path)))
				execve(full_path, cmd, env_tab);
		}
		if (!ft_strncmp(cmd[0], "./", 2))
		{
			pwd = getcwd(NULL, 256);
			full_path = ft_strnjoin(pwd, 2, "/", cmd[0] + 2);
			if (!(ret = access_exec_binary(full_path)))
				execve(full_path, cmd, env_tab);
			ft_strdel(&full_path);
		}
		else if (!(ret = access_exec_binary(cmd[0])))
			execve(cmd[0], cmd, env_tab);
		if (ret == COMMAND_NOT_FOUND)
			ft_cmd_not_found(cmd[0]);
		else if (ret == PERMISSION_DENIED)
			ft_perm_denied_msg(cmd[0]);
		exit(ret);
	}
	else
	{
		waitpid(pid, &exec->exit_status, 0);
		if (WIFEXITED(exec->exit_status))
			exec->exit_status = WEXITSTATUS(exec->exit_status);
	}
}
