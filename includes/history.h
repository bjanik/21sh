/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 17:57:36 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/18 16:42:36 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

# define INVALID_OPT 0
# define MISSING_ARG 1

typedef struct	s_history
{
	int			len;
	t_dlst		*head;
	t_dlst		*end;
	t_dlst		*current;
	char		*hist_file;
	int			fd;
}				t_history;

t_history		*init_history(void);
void			append_history(t_history *history, char *cmd, int cmd_size);
int				history_usage(char opt, int type);
int				option_d_error(char *arg);

#endif
