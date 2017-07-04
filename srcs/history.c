#include "history.h"

static void	realloc_history(t_history *history)
{
	char	**tmp;
	int	i;

	i = 0;
	tmp = history->history;
	history->history = (char**)ft_malloc(sizeof(char*) * 
		(2 * history->history_size + 1));
	while (i < history->history_len)
		history->history[i] = ft_strdup(tmp[i++]);
	history->history_size *= 2;
	ft_free_string_tab(&tmp);
	
}

void	append_history(t_history *history, char *command)
{
	if (history->history_len == history->history_size)
		history->realloc_history(history);
	history->history[history->history_len] = ft_strdup(command);	
	ft_printf("\n%s\n", history->history[history->current_pos]);
	history->history_len++;
	history->current_pos++;
}

t_history	*init_history(void)
{
	t_history	*history;

	history = (t_history*)ft_malloc(sizeof(history));
	history->history_size = INITIAL_HISTO_SIZE;
	history->history_len = 0;
	history->current_pos = 0;
	history->history = (char**)ft_malloc(sizeof(char*) *
 		(INITIAL_HISTO_SIZE + 1));
	history->realloc_history = realloc_history;
	history->append_history = append_history;
	return (history);
}
