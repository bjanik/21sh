/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 15:00:57 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/28 18:37:00 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

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

static int			exec_current_dir(char **cmd, char **env_tab, int offset)
{
	int		ret;
	char	*pwd;
	char	*full_path;

	pwd = getcwd(NULL, 256);
	full_path = ft_strnjoin(pwd, 2,  "/", cmd[0] + offset);
	if (!(ret = access_exec_binary(full_path)))
		execve(full_path, cmd, env_tab);
	return (ret);
}

static int	exec_search_in_env_path(char **cmd, t_env *env,  char **env_tab)
{
	char	**paths;
	int		ret;
	char	*full_path;
	int		i;

	i = -1;
	paths = get_cmd_path(env);
	if (paths)
	{
		while (paths[++i])
		{
			full_path = ft_strnjoin(paths[i], 2, "/", cmd[0]);
			if (!(ret = access_exec_binary(full_path)))
				execve(full_path, cmd, env_tab);
		}
	}
	ret = exec_current_dir(cmd, env_tab, 0);
	return (ret);
}

void		run_binary(t_exec *exec, t_env *env, char **cmd)
{
	int		pid;
	char	**env_tab;
	int		ret;

	((pid = fork()) < 0) ? exit(EXIT_FAILURE): 0;
	if (!pid)
	{
		restore_initial_attr(get_bsh()->term);
		(handle_redirection(exec)) ? exit(EXIT_FAILURE) : 0;
		env_tab = env_to_tab(env);
		if (cmd[0][0] == '/')
			ret = exec_absolute_path(cmd, env_tab);
		else if (!ft_strncmp("./", cmd[0],  2))
			ret = exec_current_dir(cmd, env_tab, 2);
		else
			ret = exec_search_in_env_path(cmd, env, env_tab);
		(ret == COMMAND_NOT_FOUND) ? ft_cmd_not_found(cmd[0]) :
			ft_perm_denied_msg(cmd[0]);
		exit(ret);
	}
	else
	{
		waitpid(pid, &exec->exit_status, 0);
		restore_custom_attr(get_bsh()->term);
		if (WIFEXITED(exec->exit_status))
			get_bsh()->exit_status = WEXITSTATUS(exec->exit_status);
	}
}
