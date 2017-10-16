/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/12 18:14:17 by bjanik            #+#    #+#             */
/*   Updated: 2017/10/16 16:36:32 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

static int	history_usage(char invalid_opt)
{
	ft_putstr_fd("bsh: history: -", STDERR);
	ft_putchar_fd(invalid_opt, STDERR);
	ft_putendl_fd(": invalid option", STDERR);
	ft_putendl_fd("history: usage: history [-c] [-d offset] [-n]", STDERR);
	return (1);
}

static int		display_history(t_history *hist)
{
	t_dlst		*end;
	int			cpt;

	end = hist->end;
	cpt = 1;
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

static int		history_option_d(t_dlst *head, char **cmd, int i)
{
	int		offset;
	t_dlst	*lst;

	i = (!ft_strcmp("--", cmd[i])) ? i + 1 : i;
	offset = ft_atoi(cmd[i]);
	ft_printf("OFFSET = %d\n", offset);
	while (offset-- && lst)
		lst = lst->next;
	if (!lst)
	{
		ft_putstr_fd("bsh: history: ", STDERR);
		ft_putstr_fd(cmd[i], STDERR);
		ft_putendl_fd(": history position out of range", STDERR);
		return (1);
	}
}

static int		history_option_c(t_dlst *head)
{
}

int	launch_history(char *options)
{
	int	i;

	i = -1;
	while (options[++i])
	{
	}
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
	if (cmd && cmd[0] && !cmd[1])
		return (display_history(hist));
	while (cmd[++i] && cmd[i][0] == '-' && ft_strcmp("--", cmd[i]))
	{
		if (!cmd[i][1])
		{
			ft_putendl_fd("bsh: export: `-': not a valid identifier", STDERR);
			return (1);
		}
		if ((wrong_opt = check_arg_opt(cmd[i] + 1, HISTORY_OPTIONS, options)))
			return (history_usage(wrong_opt));
	}
	if (IS_OPTION(options, 'c'))
		ft_dlstdel(&hist->head, del_hist);
	if (IS_OPTION(options, 'd'))
		history_option_d(hist->head, cmd, i);

	return (0);
}
