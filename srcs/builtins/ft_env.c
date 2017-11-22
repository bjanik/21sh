/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 12:31:28 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/22 15:51:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int	env_usage(char invalid_opt)
{
	ft_putstr_fd("bsh: env: -", STDERR);
	ft_putchar_fd(invalid_opt, STDERR);
	ft_putendl_fd(": invalid option", STDERR);
	ft_putendl_fd("env: usage: env [-ui] [name[=value] ...]",
		STDERR);
	return (1);
}

int			size_env(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

char		**env_to_tab(t_env *env)
{
	char	**tab;
	int		i;

	i = 0;
	tab = NULL;
	if (!(tab = (char**)malloc((size_env(env) + 1) * sizeof(char*))))
		exit(EXIT_FAILURE);
	while (env)
	{
		if (env->exportable)
		{
			if (!(tab[i] = (char*)malloc(sizeof(char) *
				(ft_strlen(env->var_value) + ft_strlen(env->var_name) + 2))))
				exit(EXIT_FAILURE);
			ft_strcpy(tab[i], env->var_name);
			ft_strcat(tab[i], "=");
			ft_strcat(tab[i++], env->var_value);
		}
		env = env->next;
	}
	tab[i] = NULL;
	return (tab);
}

static void	env_u_option(t_env *env, char **cmd, int *index)
{
	t_bsh	*bsh;
	t_env	*envvar;

	bsh = get_bsh();
	bsh->mod_env = mod_env(env);
	while (cmd[*index])
	{
		if ((envvar = ft_getenv(bsh->mod_env, cmd[*index])))
			envvar->exportable = 0;
		else
			break ;
		(*index)++;
	}
}

int			ft_env(t_env **env, char **cmd)
{
	char	wrong_opt;
	char	options[3];
	int		i;

	i = 0;
	ft_bzero(options, 3);
	if (cmd && cmd[0] && !cmd[1])
		return (display_env(*env));
	while (cmd[++i] && cmd[i][0] == '-' && ft_strcmp("--", cmd[i]))
	{
		(!cmd[i][1]) ? options[0] = 'i' : 0;
		if ((wrong_opt = check_arg_opt(cmd[i] + 1, "iu", options)))
			return (env_usage(wrong_opt));
	}
	if (IS_OPTION(options, 'i'))
	{
		while (cmd[i] && ft_strchr(cmd[i], '='))
			set_var(&get_bsh()->mod_env, cmd[i++], EXPORT_VAR);
	}
	else if (IS_OPTION(options, 'u'))
		env_u_option(*env, cmd, &i);
	return (0);
}
