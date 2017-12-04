/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_lessand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/20 16:44:39 by bjanik            #+#    #+#             */
/*   Updated: 2017/12/04 14:19:38 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int		redir_lessand(t_redir *redir)
{
	int	fd;

	if (!ft_strcmp(redir->filename, "-"))
		close(STDIN);
	else if (check_access_exist(redir->filename) ||
			check_access_reading(redir->filename))
		return (1);
	if (ft_str_isdigit(redir->filename))
	{
		fd = ft_atoi(redir->filename);
		if (dup2(fd, STDIN) < 0)
			return (1);
	}
	return (0);
}
