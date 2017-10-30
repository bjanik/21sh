/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 17:37:17 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/23 19:53:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int		ft_exit(t_env **env, char **cmd)
{
	int	exit_val;

	(void)env;
	exit_val = 0;
	if (cmd[1] && cmd[2])
		ft_putendl_fd("bsh: exit: too many arguments", STDERR);
	else if (cmd[1] && !ft_str_isdigit(cmd[1]))
	{
		ft_putendl_fd("bsh: exit: numeric argument required", STDERR);
		return (1);
	}
	if (cmd[1])
		exit_val = ft_atoi(cmd[1]);
	ft_printf("exit\n");
	exit(exit_val % 0x100);
	return (0);
}
