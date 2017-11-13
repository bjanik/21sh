/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 18:14:17 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/13 14:48:16 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int		display_history(t_history *hist, char **cmd)
{
	t_dlst		*end;
	int			cpt;
	int			offset;

	end = hist->end;
	cpt = 0;
	if (cmd[1] && ft_str_isdigit(cmd[1]))
	{
		end = hist->head;
		offset = ft_atoi(cmd[1]);
		if (hist->len > offset)
			cpt = hist->len - offset;
		while (end && --offset)
			end = end->next;
	}
	if (!end)
		end = hist->end;
	while (end)
	{
		ft_printf("%d  %s\n", cpt++, (char*)end->data);
		end = end->prev;
	}
	return (0);
}

void			del_hist(void *data, size_t data_size)
{
	(void)data_size;
	ft_memdel(&data);
}

static int		history_option_d(t_dlst **end, char **cmd, int i)
{
	int		offset;
	t_dlst	*lst;

	lst = *end;
	if (!*end)
		return (1);
	if (!cmd[i])
		return (history_usage('d', MISSING_ARG));
	i = (!ft_strcmp("--", cmd[i])) ? i + 1 : i;
	offset = ft_atoi(cmd[i]);
	if (!offset || ft_abs(offset) > get_bsh()->history->len)
		return (option_d_error(cmd[i]));
	while (--offset && lst)
		lst = lst->prev;
	if (!lst->prev)
	{
		get_bsh()->history->head = lst->next;
		ft_memdel(&lst->data);
		ft_memdel((void**)&lst);
	}
	else if (!lst->next)
	{
		lst->prev->next = NULL;
		*end = lst->prev;
		ft_memdel(&lst->data);
		ft_memdel((void**)&lst);
	}
	else
	{
		lst->prev->next = lst->next;
		lst->next->prev = lst->prev;
		ft_memdel(&lst->data);
		ft_memdel((void**)lst);
	}
	return (0);
}

int				ft_history(t_env **env, char **cmd)
{
	t_history	*hist;
	char		options[5];
	char		wrong_opt;
	int			i;

	(void)env;
	hist = get_bsh()->history;
	i = 0;
	//if (cmd && cmd[0] && !cmd[1])
	//	return (display_history(hist));
	while (cmd[++i] && cmd[i][0] == '-' && ft_strcmp("--", cmd[i]))
	{
		if (!cmd[i][1])
		{
			ft_putendl_fd("bsh: history: `-': not a valid identifier", STDERR);
			return (1);
		}
		if ((wrong_opt = check_arg_opt(cmd[i] + 1, HISTORY_OPTIONS, options)))
			return (history_usage(wrong_opt, INVALID_OPT));
	}
	if (IS_OPTION(options, 'c'))
	{
		ft_dlstdel(&hist->head, del_hist);
		hist->end = NULL;
	}
	else if (IS_OPTION(options, 'd'))
		history_option_d(&hist->end, cmd, i);
	(i == 1) ? display_history(hist, cmd) : 0;
	return (0);
}
