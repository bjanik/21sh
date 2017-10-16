/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:56:16 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/12 18:23:46 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"
#include "builtins.h"

void		append_history(t_history *history, char *cmd, int cmd_size)
{
	t_dlst	*hist;

	hist = ft_dlstnew(cmd, cmd_size);
	ft_dlstadd(&history->head, hist);
	if (history->end == NULL)
		history->end = history->head;
	history->len++;
	history->current = NULL;
}

t_history	*init_history(void)
{
	t_history	*history;

	history = (t_history*)ft_malloc(sizeof(t_history));
	history->head = NULL;
	history->end = NULL;
	history->current = NULL;
	history->len = 0;
	history->fd = open("~/.21sh_history", O_CREAT | O_RDWR | O_APPEND, 0644);
	return (history);
}
