/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 18:57:47 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/25 15:51:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int		update_working_directories(char *dir, t_env *env)
{
	char	*wd[2];
	t_env	*e;

	if ((wd[0] = getcwd(NULL, 256)))
		wd[0] = ft_strjoin_free("OLDPWD=", wd[0], 2);
	chdir(dir);
	if ((wd[1] = getcwd(NULL, 256)))
		wd[1] = ft_strjoin_free("PWD=", wd[1], 2);
	if ((e = ft_getenv(env, "OLDPWD")))
		set_var(&env, wd[0], e->exportable);
	else
		set_var(&env, wd[0], EXPORT_VAR);
	if ((e = ft_getenv(env, "PWD")))
		set_var(&env, wd[1], e->exportable);
	else
		set_var(&env, wd[1], EXPORT_VAR);
	ft_strdel(&wd[0]);
	ft_strdel(&wd[1]);
	return (0);
}

static int		check_and_change_dir(char *dir, t_env *env)
{
	struct stat	info;

	stat(dir, &info);
	if (access(dir, F_OK))
	{
		ft_putstr_fd("bsh: ", STDERR);
		ft_putstr_fd(dir, STDERR);
		ft_putendl_fd(": cd: No such file or directory", STDERR);
	}
	else if (!S_ISDIR(info.st_mode))
	{
		ft_putstr_fd("bsh: ", STDERR);
		ft_putstr_fd(dir, STDERR);
		ft_putendl_fd(": cd: Not a directory", STDERR);
	}
	else if (access(dir, X_OK))
	{
		ft_putstr_fd("bsh: ", STDERR);
		ft_putstr_fd(dir, STDERR);
		ft_putendl_fd(": cd: Permission denied", STDERR);
	}
	else
		return (update_working_directories(dir, env));
	return (1);
}

int				ft_cd(t_env **env, char **cmd)
{
	t_env	*e;

	if (cmd[1] && cmd[2])
		ft_putendl_fd("bsh: cd: too many arguments", STDERR);
	else if (cmd[0] && !cmd[1])
	{
		if ((e = ft_getenv(*env, "HOME")))
			return (check_and_change_dir(e->var_value, *env));
		else
			ft_putendl_fd("bsh: cd: HOME not set", STDERR);
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		if ((e = ft_getenv(*env, "OLDPWD")))
			return (check_and_change_dir(e->var_value, *env));
		else
			ft_putendl_fd("bsh: cd: OLDPWD not set", STDERR);
	}
	else
		return (check_and_change_dir(cmd[1], *env));
	return (1);
}
