/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/17 16:20:47 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 18:59:59 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	history_usage(char opt, int type)
{
	ft_putstr_fd("bsh: history: -", STDERR);
	ft_putchar_fd(opt, STDERR);
	(type == INVALID_OPT) ? ft_putendl_fd(": invalid option", STDERR) :
		ft_putendl_fd(": option requires an argument", STDERR);
	ft_putendl_fd("history: usage: history [-c] [-d offset] [-n]", STDERR);
	return (1);
}

int	option_d_error(char *arg)
{
	ft_putstr_fd("bsh: history: ", STDERR);
	ft_putstr_fd(arg, STDERR);
	ft_putendl_fd(": history position out of range", STDERR);
	return (1);
}
