/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 18:27:24 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/02 14:42:05 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_echo(t_env **env, char **cmd)
{
	int	new_line;
	int	i;

	(void)env;
	new_line = 1;
	i = 1;
	if (cmd[1] && !ft_strcmp("-n", cmd[1]))
	{
		new_line = 0;
		i++;
	}
	while ( cmd[i])
		ft_putstr(cmd[i++]);
	(new_line) ? ft_putchar('\n') : 0;
	return (0);
}
