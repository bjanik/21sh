/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 18:42:33 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/28 18:42:34 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_redir	*init_redir(void)
{
	t_redir	*redir;

	if (!(redir = (t_redir*)malloc(sizeof(t_redir))))
		exit(EXIT_FAILURE);
	redir->next = NULL;
	redir->filename = NULL;
	redir->here_end = NULL;
	redir->fd = STDOUT;
	return (redir);
}
