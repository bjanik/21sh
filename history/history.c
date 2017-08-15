/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 14:56:16 by bjanik            #+#    #+#             */
/*   Updated: 2017/07/05 15:50:55 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

/*static void	realloc_history(t_history *history)
{
	char	**tmp;
	int		i;

	i = -1;
	tmp = history->history;
	history->history = (char**)ft_malloc(sizeof(char*) *
		(2 * history->size + 1));
	while (++i < history->len)
		history->history[i] = ft_strdup(tmp[i]);
	history->size *= 2;
	ft_free_string_tab(&tmp);
}*/

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
	history->append_history = append_history;
	return (history);
}
