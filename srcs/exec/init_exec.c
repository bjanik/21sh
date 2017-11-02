/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/28 18:42:28 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/02 12:40:32 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

t_exec	*init_exec(void)
{
	t_exec	*exec;

	if (!(exec = (t_exec*)malloc(sizeof(t_exec))))
		exit(EXIT_FAILURE);
	exec->word_list = NULL;
	exec->last_word = NULL;
	exec->word_count = 0;
	exec->cmd = NULL;
	exec->redir_list = NULL;
	exec->last_redir = NULL;
	exec->is_builtin = -1;
	exec->cmd_separator = 0;
	exec->exit_status = 0;
	exec->next = NULL;
	exec->prev = NULL;
	return (exec);
}
