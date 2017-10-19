/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/17 18:48:07 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/19 17:36:12 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int		check_access_exist(char *file)
{
	if (access(file, F_OK))
		return (ft_no_file_msg(file));
	return (0);
}

int		check_access_writing(char *file)
{
	if (access(file, W_OK))
		return (ft_perm_denied_msg(file));
	return (0);
}

int		check_access_reading(char *file)
{
	if (access(file, R_OK))
		return (ft_perm_denied_msg(file));
	return (0);
}
