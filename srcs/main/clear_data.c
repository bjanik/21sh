/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/19 16:04:13 by bjanik            #+#    #+#             */
/*   Updated: 2017/11/08 19:10:39 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsh.h"

void		clear_token_list(t_token **token)
{
	t_token	*lst;

	lst = *token;
	while (*token)
	{
		lst = (*token)->next;
		ft_strdel(&(*token)->token);
		(*token)->next = NULL;
		(*token)->prev = NULL;
		ft_memdel((void**)token);
		*token = lst;
	}
}

static void	del(void *content, size_t size)
{
	(void)size;
	ft_memdel(&content);
}

void		clear_pipes(t_pipes *pipes)
{
	int		i;

	i = -1;
	while (++i < pipes->nb_pipes)
		ft_memdel((void**)&(pipes->pipes_fd[i]));
	ft_memdel((void**)pipes);
	pipes->nb_pipes = 0;
}

void		clear_exec(t_exec **exec)
{
	t_exec	*ex;
	t_redir	*rd;
	t_list	*wd;

	if (!exec || !*exec)
		return ;
	ex = *exec;
	wd = ex->word_list;
	while (ex)
	{
		ft_lstdel(&ex->word_list, del);
		ft_free_string_tab(&ex->cmd);
		ex->last_word = NULL;
		rd = ex->redir_list;
		ex->prev = NULL;
		ex->last_redir = NULL;
		while (rd)
		{
			ex->redir_list = rd->next;
			ft_strdel(&rd->filename);
			ft_strdel(&rd->here_end);
			ft_lstdel(&rd->heredoc_input[0], del);
			rd->heredoc_input[1] = NULL;
			rd->next = NULL;
			ft_memdel((void**)&rd);
			rd = ex->redir_list;
		}
		ex = ex->next;
		ft_memdel((void**)exec);
		*exec = ex;
	}
	clear_pipes(get_bsh()->pipes);
}
