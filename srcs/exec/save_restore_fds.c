/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_restore_fds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 15:36:59 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/18 15:40:07 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void	save_fds(int *saved_fd)
{
	saved_fd[0] = dup(STDIN);
	saved_fd[1] = dup(STDOUT);
	saved_fd[2] = dup(STDERR);
}

void	restore_fds(int *saved_fd)
{
	dup2(saved_fd[0], STDIN);
	dup2(saved_fd[1], STDOUT);
	dup2(saved_fd[2], STDERR);
}
