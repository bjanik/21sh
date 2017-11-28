/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 18:42:41 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/27 19:58:06 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

int		redir_great(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (!access(redir->filename, F_OK) &&
			check_access_writing(redir->filename))
		return (1);
	if ((fd = open(redir->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
		return (1);
	if (dup2(fd, redir->fd) < 0)
	{
		ft_putendl_fd("dup2 failed\n", STDERR);
		return (1);
	}
	close(fd);
	return (0);
}

int		redir_dgreat(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (!access(redir->filename, F_OK) &&
			check_access_writing(redir->filename))
		return (1);
	if ((fd = open(redir->filename, O_CREAT | O_WRONLY | O_APPEND, 0644)) < 0)
		return (1);
	if (dup2(fd, redir->fd) < 0)
	{
		ft_putendl_fd("dup2 failed\n", STDERR);
		return (1);
	}
	close(fd);
	return (0);
}

int		redir_less(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (check_access_exist(redir->filename) ||
			check_access_reading(redir->filename))
		return (1);
	if ((fd = open(redir->filename, O_RDONLY, 0644)) < 0)
		return (1);
	dup2(fd, STDIN);
	if (dup2(fd, STDIN) < 0)
	{
		ft_putendl_fd("dup2 failed\n", STDERR);
		return (1);
	}
	close(fd);
	return (0);
}
