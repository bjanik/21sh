/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 14:47:08 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/23 18:15:21 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
