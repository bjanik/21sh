/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_greatand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 15:17:24 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/05 20:09:30 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int	redir_greatand(t_redir *redir)
{
	int	dest_fd;

	dest_fd = ft_atoi(redir->filename);
	if (!ft_strcmp(redir->filename, "-"))
		close(redir->fd);
	else if (redir->fd == STDOUT && !ft_str_isdigit(redir->filename))
		redir_great(redir);
	else if (ft_str_isdigit(redir->filename))
	{
		if (dest_fd > 2 || dup2(dest_fd, redir->fd) < 0)
		{
			ft_putendl_fd("bsh: Bad file descriptor", STDERR);
			return (1);
		}
	}
	else
		return (ambigous_redirect(redir->filename));
	return (0);
}
