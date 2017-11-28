/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 15:00:57 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/27 19:49:22 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"
#define BACK_UP_PATH "/bin:/usr/bin"

static int	access_exec_binary(char *bin_path)
{
	if (access(bin_path, F_OK))
		return (COMMAND_NOT_FOUND);
	else if (access(bin_path, X_OK))
		return (PERMISSION_DENIED);
	return (0);
}

static int	exec_absolute_path(char **cmd, char **env_tab)
{
	int		ret;

	if (!(ret = access_exec_binary(cmd[0])))
		execve(cmd[0], cmd, env_tab);
	return (ret);
}

static int	exec_current_dir(char **cmd, char **env_tab)
{
	int		ret;
	char	*pwd;
	char	*full_path;

	pwd = getcwd(NULL, 256);
	full_path = ft_strnjoin(pwd, 2, "/", cmd[0]);
	if (!(ret = access_exec_binary(full_path)))
		execve(full_path, cmd, env_tab);
	return (ret);
}

static int	exec_search_in_env_path(char **cmd, t_env *env, char **env_tab)
{
	char	**paths;
	int		ret;
	char	*full_path;
	int		i;

	i = -1;
	paths = get_cmd_path(env);
	if (!paths)
		paths = ft_strsplit(BACK_UP_PATH, ':');
	ret = 0;
	while (paths[++i])
	{
		full_path = ft_strnjoin(paths[i], 2, "/", cmd[0]);
		if (!(ret = access_exec_binary(full_path)))
			execve(full_path, cmd, env_tab);
	}
	return (ret);
}

void		run_binary(t_exec *exec, t_env *env, int offset)
{
	char	**env_tab;
	int		ret;

	signal(SIGINT, SIG_DFL);
	(handle_redirection(exec)) ? exit(EXIT_FAILURE) : 0;
	env_tab = env_to_tab(env);
	if (exec->cmd[offset][0] == '/')
		ret = exec_absolute_path(exec->cmd + offset, env_tab);
	else if (!ft_strncmp("./", exec->cmd[offset], 2))
		ret = exec_current_dir(exec->cmd + offset, env_tab);
	else
		ret = exec_search_in_env_path(exec->cmd + offset, env, env_tab);
	(ret == COMMAND_NOT_FOUND) ? ft_cmd_not_found(exec->cmd[offset]) :
		ft_perm_denied_msg(exec->cmd[offset]);
	exit(ret);
}
