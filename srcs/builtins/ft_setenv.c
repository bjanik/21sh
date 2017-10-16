/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 18:25:06 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/14 17:11:56 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	setenv_no_value(t_env *env, char **split, char *var, int export)
{
	t_env	*e;

	e = NULL;
	if (!split[1] && !ft_strchr(var, '='))
	{
		if (export && (e = ft_getenv(env, var)))
			e->exportable = 1;
		ft_free_string_tab(&split);
		return (1);
	}
	return (0);
}

static int	set_new_var(t_env *ptr, char **spvar)
{
	if (!spvar[1] && !ft_strcmp(ptr->var_name, spvar[0]))
	{
		ft_free_string_tab(&spvar);
		ft_strdel(&ptr->var_value);
		ptr->var_value = ft_strdup("");
		return (1);
	}
	if (spvar[1] && !ft_strcmp(ptr->var_name, spvar[0]))
	{
		ft_strdel(&ptr->var_value);
		ptr->var_value = (spvar[1]) ? ft_strdup(spvar[1]) : ft_strdup("");
		ft_free_string_tab(&spvar);
		return (1);
	}
	return (0);
}

void		set_var(t_env **env, char *var, int export)
{
	char	**spvar;
	t_env	*ptr;

	ptr = *env;
	spvar = ft_strsplit(var, '=');
	if (!spvar || !spvar[0] || var[0] == '=')
	{
		ft_free_string_tab(&spvar);
		return ;
	}
	if (!var || setenv_no_value(*env, spvar, var, export) == 1)
		return ;
	while (ptr)
	{
		if (set_new_var(ptr, spvar))
			return ;
		ptr = ptr->next;
	}
	push_back_env(env, var, export);
	ft_free_string_tab(&spvar);
}

int			ft_setenv(t_env **env, char **cmd)
{
	int	i;

	i = 1;
	if (cmd[0] && !cmd[1])
	{
		ft_putendl_fd("setenv : not enough arguments", 2);
		return (-1);
	}
	while (cmd[i])
		set_var(env, cmd[i++], 0);
	return (0);
}