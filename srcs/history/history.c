/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:56:16 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/06 14:03:25 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"
#include "builtins.h"

void			append_history(t_history *history, char *cmd, int cmd_len)
{
	t_dlst		*hist;

	hist = ft_dlstnew(cmd, cmd_len + 1);
	ft_dlstadd(&history->head, hist);
	if (history->end == NULL)
		history->end = history->head;
	history->len++;
	history->current = NULL;
}

t_history		*init_history(void)
{
	t_history	*history;

	if (!(history = (t_history*)malloc(sizeof(t_history))))
		return (NULL);
	history->head = NULL;
	history->end = NULL;
	history->current = NULL;
	history->len = 0;
	history->fd = -1;
	return (history);
}
