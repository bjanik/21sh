/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 14:47:08 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/12 19:54:35 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/*static t_builtins	g_builtins[] = {
	{ "cd", ft_cd},
	{ "echo", ft_echo},
	{ "env", ft_env},
	{ "setenv", ft_setenv},
	{ "unsetenv", ft_unsetenv},
	{ "export", ft_export},
	{ "history", ft_history},
	{ NULL, NULL},
};*/

int			cmd_is_builtin(char **cmd)
{
	int		i;

	i = -1;
	while (cmd && g_builtins[++i].b_name)
	{
		if (!ft_strcmp(g_builtins[i].b_name, cmd[0]))
			return (i);
	}
	return (-1);
}
