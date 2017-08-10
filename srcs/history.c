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

static void	realloc_history(t_history *history)
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
}

void		append_history(t_history *history, char *command)
{
	if (history->len == history->size)
		history->realloc_history(history);
	history->history[history->len] = ft_strndup(command, ft_strlen(command) - 1);
	history->len++;
	history->current_pos++;
}

t_history	*init_history(void)
{
	t_history	*history;
	int			i;

	i = 0;
	history = (t_history*)ft_malloc(sizeof(t_history));
	history->size = INITIAL_HISTO_SIZE;
	history->current_pos = 0;
	history->len = 0;
	history->history = (char**)ft_malloc(sizeof(char*) *
			(INITIAL_HISTO_SIZE + 1));
	while (i < history->size)
		history->history[i++] = NULL;
	history->append_history = append_history;
	history->realloc_history = realloc_history;
	return (history);
}
